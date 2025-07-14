//
// Created by os on 6/22/24.
//
#include "../h/MemoryAllocator.hpp"
#include "../lib/mem.h"

MemoryAllocator::memSeg *MemoryAllocator::freeSegHead = nullptr;
size_t MemoryAllocator::memSegSize = sizeof(MemoryAllocator::memSeg);

void MemoryAllocator::initilaze () {

    MemoryAllocator::freeSegHead = (memSeg *) HEAP_START_ADDR;
    freeSegHead->numOfBlocks = ((uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR) / MEM_BLOCK_SIZE;
    freeSegHead->next = nullptr;
}

void *MemoryAllocator::mem_alloc (size_t numOfBlocks) {

    if (numOfBlocks < 0) return nullptr;
    memSeg *curr = MemoryAllocator::freeSegHead, *prev = nullptr;

    //Jumps out when place is found or when there is no free space (curr is nullptr)
    // Using algorithm first fit
    for (; curr != nullptr && numOfBlocks > curr->numOfBlocks; prev = curr, curr = curr->next);

    if (curr == nullptr) {
        return nullptr;
    }

    if (numOfBlocks < curr->numOfBlocks) {
        //At least one block left unallocated
        size_t newSeg = (size_t) curr + numOfBlocks * MEM_BLOCK_SIZE;  //starting address of a newSeg
        memSeg *tmp = (memSeg *) newSeg;
        tmp->numOfBlocks = curr->numOfBlocks - numOfBlocks;  //remaining number of blocks
        tmp->next = curr->next;
        tmp->prev = curr->prev;
        if(tmp->next) tmp->next->prev = tmp;
        if (prev) prev->next = tmp;
        else {
            MemoryAllocator::freeSegHead = tmp;
        }

    } else {
        //No blocks left to allocate, exact equal size
        if (prev) prev->next = curr->next;
        else MemoryAllocator::freeSegHead = curr->next;

    }
    curr->numOfBlocks = numOfBlocks;
    memSeg* shifted = curr + 1;
    return (void*) shifted; // return curr moved for sizeof(segHeader);
}

int MemoryAllocator::mem_free (void *p) {


    memSeg *tmp = (memSeg *) p - 1;

    // No free space, what we dealoc will be only free space on heap
    if (!freeSegHead) {
        tmp->next = nullptr;
        tmp->prev = nullptr;
        MemoryAllocator::freeSegHead = tmp;
        return 0;
    }

    memSeg *curr = freeSegHead;

    //find the place for segment in the list
    for (; curr < tmp && curr->next != nullptr; curr = curr->next);

    //case that p is already free
    if(curr != freeSegHead){
        if((size_t)curr->prev + (size_t)curr->prev->numOfBlocks * MEM_BLOCK_SIZE > (size_t)curr)
            return -1;
    }


    //segment is first in the list
    if (curr == freeSegHead) {
        curr->prev = tmp;
        tmp->prev = nullptr;
        tmp->next = curr;
        MemoryAllocator::freeSegHead = tmp;
        tryToMerge(tmp, tmp->next);
        return 0;
    } else {
        //segment is last in the list
        if (tmp > curr) {
            curr->next = tmp;
            tmp->prev = curr;
            tmp->next = nullptr;
            tryToMerge(tmp->prev, tmp);
            return 0;
        } else {
            //segment is somewhere in the middle
            tmp->next = curr;
            tmp->prev = curr->prev;
            curr->prev->next = tmp;
            curr->prev = tmp;
            tryToMerge(tmp, tmp->next);
            tryToMerge(tmp->prev, tmp);
            return 0;

        }
    }

}

void MemoryAllocator::tryToMerge (memSeg *main, memSeg *to_merge) {
    if (main + main->numOfBlocks == to_merge) {
        //Needs to be merged
        main->numOfBlocks += to_merge->numOfBlocks;
        main->next = to_merge->next;
        to_merge->next->prev = main;

    }

}