/*
 *  ANCustomAllocation.h
 *  MyAlloc
 *
 *  Created by Alex Nichol on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <CoreFoundation/CoreFoundation.h>
#import "ANBuddyAlloc.h"

const void * CustomAllocatorRetainCall (const void * info);
void CustomAllocatorReleaseCall (const void * info);
CFStringRef CustomAllocatorCreateDescription (const void * info);
void * CustomAllocatorAllocate (CFIndex allocSize, CFOptionFlags hint, void * info);
void * CustomAllocatorReallocate (void * ptr, CFIndex newsize, 
								  CFOptionFlags hint, void * info);
void CustomAllocatorDeallocate (void * ptr, void * info);
CFIndex CustomAllocatorPreferedSize (CFIndex size, CFOptionFlags hint, void * info);
CFAllocatorRef AllocatorWithBuddyHeap (ANMemoryHeap * heap);
CFAllocatorRef AllocatorWithNewHeap ();
