//
//  ANBuddyAlloc.h
//  BuddyAllocator
//
//  Created by Alex Nichol on 10/19/10.
//  Copyright (c) 2010 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef bool
#define bool int
#define TRUE 1
#define FALSE 0
#endif

typedef struct {
    bool taken;
    int size;
    void * ptr;
    // tree
	void * left;
    void * right;
    void * parent;
} ANMemoryNode;

typedef struct {
    ANMemoryNode * table;
    int totalBytes;
    void * ptr;
	void * userInfo;
	int retainCount;
} ANMemoryHeap;

// ANMemoryNode
ANMemoryNode * ANMemoryNodeInit ();
bool ANMemoryNodeIsBottom (ANMemoryNode * node);
void ANMemoryNodeFree (ANMemoryNode * node, bool subnodes);

// ANMemoryHeap
ANMemoryHeap * ANMemoryHeapCreate (int totalSize);
void * ANMemoryNodeSplitToSize (ANMemoryNode * node, int size);
void * ANMemoryHeapGetMemory (ANMemoryHeap * heap, int size);
void ANMemoryNodeFreeMemory (ANMemoryNode * node, void * ptr);
void ANMemoryHeapFreeMemory (ANMemoryHeap * heap, void * ptr);
bool ANMemoryNodeDefragment (ANMemoryNode * node);
void ANMemoryHeapDefragment (ANMemoryHeap * heap);
void ANMemoryHeapFree (ANMemoryHeap * heap);
int ANMmeoryNodeGetSize (ANMemoryNode * node, void * ptr);
int ANMemoryHeapGetSize (ANMemoryHeap * heap, void * ptr);
