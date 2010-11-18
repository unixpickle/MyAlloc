/*
 *  ANCustomAllocation.c
 *  MyAlloc
 *
 *  Created by Alex Nichol on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ANCustomAllocation.h"

const void * CustomAllocatorRetainCall (const void * info) {
	((ANMemoryHeap *)info)->retainCount += 1;
	return info;
}

void CustomAllocatorReleaseCall (const void * info) {
	((ANMemoryHeap *)info)->retainCount -= 1;
	if (((ANMemoryHeap *)info)->retainCount == 0) {
		ANMemoryHeapFree((ANMemoryHeap *)info);
	}
}

CFStringRef CustomAllocatorCreateDescription (const void * info) {
	char myBytes[512];
	sprintf(myBytes, "BuddyAllocator: %p", ((ANMemoryHeap *)info)->userInfo);;
	return CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)myBytes, strlen(myBytes),
								   kCFStringEncodingASCII,
								   FALSE);
}

void * CustomAllocatorAllocate (CFIndex allocSize, CFOptionFlags hint, void * info) {
	ANMemoryHeap * heap = (ANMemoryHeap *)info;
	void * data = ANMemoryHeapGetMemory(heap, allocSize);
	return data;
}

void * CustomAllocatorReallocate (void * ptr, CFIndex newsize, 
								  CFOptionFlags hint, void * info) {
	ANMemoryHeap * heap = (ANMemoryHeap *)info;
	void * nptr = ANMemoryHeapGetMemory(heap, newsize);
	memcpy(nptr, ptr, ANMemoryHeapGetSize(heap, ptr));
	ANMemoryHeapFreeMemory(heap, ptr);
	return nptr;
}

void CustomAllocatorDeallocate (void * ptr, void * info) {
	ANMemoryHeap * heap = (ANMemoryHeap *)info;
	ANMemoryHeapFreeMemory(heap, ptr);
}

CFIndex CustomAllocatorPreferedSize (CFIndex size, CFOptionFlags hint, void * info) {
	// basically buddy allocation method.
	ANMemoryHeap * heap = (ANMemoryHeap *)info;
	int i = heap->totalBytes;
	while (i >= size) {
		i /= 2;
	}
	return (CFIndex)(i * 2);
}

CFAllocatorRef AllocatorWithBuddyHeap (ANMemoryHeap * heap) {
	CFAllocatorContext * context = (CFAllocatorContext *)malloc(sizeof(CFAllocatorContext));
	context->info = heap;
	context->version = 0;
	context->retain = CustomAllocatorRetainCall;
	context->release = CustomAllocatorReleaseCall;
	context->copyDescription = CustomAllocatorCreateDescription;
	context->allocate = CustomAllocatorAllocate;
	context->reallocate = CustomAllocatorReallocate;
	context->deallocate = CustomAllocatorDeallocate;
	context->preferredSize = CustomAllocatorPreferedSize;
	CFAllocatorRef allocator = CFAllocatorCreate(kCFAllocatorDefault, context);
	heap->userInfo = (void *)allocator;
	return allocator;
}

CFAllocatorRef AllocatorWithNewHeap () {
	int buddyHeapSize = (1024*1024) * 10; // 10mb
	ANMemoryHeap * heap = ANMemoryHeapCreate(buddyHeapSize);
	heap->userInfo = nil;
	return AllocatorWithBuddyHeap(heap);
}