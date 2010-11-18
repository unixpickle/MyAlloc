//
//  ANBuddyAlloc.c
//  BuddyAllocator
//
//  Created by Alex Nichol on 10/19/10.
//  Copyright (c) 2010 __MyCompanyName__. All rights reserved.
//

#include "ANBuddyAlloc.h"

// ANMemoryNode
ANMemoryNode * ANMemoryNodeInit () {
    ANMemoryNode * node = (ANMemoryNode *)malloc(sizeof(ANMemoryNode));
	bzero(node, sizeof(ANMemoryNode));
	node->taken = FALSE;
	//printf("Alloc node: %p\n", node);
    return node;
}
bool ANMemoryNodeIsBottom (ANMemoryNode * node) {
    return !(node->left || node->right);
}
void ANMemoryNodeFree (ANMemoryNode * node, bool subnodes) {
    if (subnodes) {
        if (node->left) ANMemoryNodeFree(node->left, subnodes);
        if (node->right) ANMemoryNodeFree(node->right, subnodes);
    }
    free(node);
}

// ANMemoryHeap
ANMemoryHeap * ANMemoryHeapCreate (int totalSize) {
    ANMemoryHeap * heap = (ANMemoryHeap *)malloc(sizeof(ANMemoryHeap));
	bzero(heap, sizeof(heap));
    heap->totalBytes = totalSize;
    heap->ptr = (void *)malloc(totalSize);
    heap->table = ANMemoryNodeInit();
	heap->table->size = totalSize;
	heap->table->ptr = heap->ptr;
	heap->table->taken = FALSE;
	heap->userInfo = 0;
	heap->retainCount = 0;
    return heap;
}

void * ANMemoryNodeSplitToSize (ANMemoryNode * node, int size) {
    if (node->taken) return NULL;
    if (size == 1) return NULL;
    if (node->size < size) return NULL;
    if (node->size / 2 >= size) {
        // return some interesting stuff
        if (ANMemoryNodeIsBottom(node)) {
            // split up
            ANMemoryNode * node1 = ANMemoryNodeInit();
            ANMemoryNode * node2 = ANMemoryNodeInit();
            node1->size = node->size / 2;
            node2->size = node->size / 2;
            node1->ptr = node->ptr;
            node2->ptr = node->ptr + (node->size / 2);
            node->left = node1;
            node->right = node2;
            node1->parent = node;
            node2->parent = node;
        }
        void * data = ANMemoryNodeSplitToSize(node->left, size);
        if (data) return data;
        else return ANMemoryNodeSplitToSize(node->right, size);
    } else if (node->size >= size) {
        node->taken = TRUE;
        return node->ptr;
    }
	return NULL;
}
void * ANMemoryHeapGetMemory (ANMemoryHeap * heap, int size) {
    return ANMemoryNodeSplitToSize(heap->table, size);
}
void ANMemoryNodeFreeMemory (ANMemoryNode * node, void * ptr) {
    // basically recurse each node until we find the ptr.
    // then free it and set taken to FALSE.
    if (node->ptr != ptr || !ANMemoryNodeIsBottom(node)) {
        if (node->left) {
            ANMemoryNodeFreeMemory (node->left, ptr);
        }
        if (node->right) {
            ANMemoryNodeFreeMemory (node->right, ptr);
        }
    } else {
        node->taken = FALSE;
        return;
    }
}
void ANMemoryHeapFreeMemory (ANMemoryHeap * heap, void * ptr) {
    ANMemoryNodeFreeMemory(heap->table, ptr);
    ANMemoryHeapDefragment(heap);
}
bool ANMemoryNodeDefragment (ANMemoryNode * node) {
    if (ANMemoryNodeIsBottom(node)) {
        if (node->taken == TRUE) {
			return FALSE;
		}
        return TRUE;  
    } else {
        if (node->left) {
            if (!ANMemoryNodeDefragment(node->left)) {
				//printf("RET FALSE");
				return FALSE;
			}
        }
        if (node->right) {
            if (!ANMemoryNodeDefragment(node->right)) {
				//printf("RET FALSE");
				return FALSE;
				
			}
        }
        ANMemoryNodeFree(node->left, TRUE);
        ANMemoryNodeFree(node->right, TRUE);
        node->left = NULL;
        node->right = NULL;
    }
    return TRUE;
}
void ANMemoryHeapDefragment (ANMemoryHeap * heap) {
    ANMemoryNodeDefragment(heap->table);
}
int ANMemoryNodeGetSize (ANMemoryNode * node, void * ptr) {
	// basically recurse each node until we find the ptr.
    // then free it and set taken to FALSE.
    if (node->ptr != ptr || !ANMemoryNodeIsBottom(node)) {
        if (node->left) {
            int i = ANMemoryNodeGetSize(node->left, ptr);
			if (i >= 0) return i;
        }
        if (node->right) {
            int i = ANMemoryNodeGetSize(node->right, ptr);
			if (i >= 0) return i;
        }
    } else {
        //node->taken = FALSE;
        return node->size;
    }
	return -1;
}
int ANMemoryHeapGetSize (ANMemoryHeap * heap, void * ptr) {
	return ANMemoryNodeGetSize(heap->table, ptr);
}
void ANMemoryHeapFree (ANMemoryHeap * heap) {
    free(heap->ptr);
    ANMemoryNodeFree(heap->table, TRUE);
    free(heap);
}
