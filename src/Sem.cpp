//
// Created by os on 6/26/24.
//
#include "../h/Sem.hpp"
#include "../h/MemoryAllocator.hpp"

void Sem::block() {
    if (head != nullptr){
        tail->nextSem = TCB::running;
        tail = TCB::running;
    }
    else{
        head = TCB::running;
        tail = TCB::running;
    }
    TCB::running->setBlocked(true);
    TCB::dispatch();
}

void Sem::unblock() {
    if (head == nullptr) tail = nullptr;
    else {
        TCB* tmp = head;
        head = head->nextSem;
        tmp->nextSem = nullptr;
        tmp->setBlocked(false);
        Scheduler::put(tmp);
    }
}

int Sem::wait() {
    if(--val < 0) block();
    if (isDealocated()) return -1;
    return 0;
}

void Sem::signal() {
    if(++val <= 0) unblock();
}

int Sem::tryWait() {
    if(--val < 0) return 0;
    else if(val >= 0) return 1;
    else return -1;
}

Sem* Sem::createSem(uint64 init) {
    return new Sem(init);
}

int Sem::closeSem(Sem *sem ) {
    if(sem == nullptr) return -1;
    sem->setDealoc(true);
    int i = sem->getValue();
    for (;i <= 0; i++) sem->signal();

    return 0;
}





void *Sem::operator new(size_t size) {
    size = ((size + MemoryAllocator::memSegSize) % MEM_BLOCK_SIZE == 0 ? (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE :
            (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE + 1);

    return MemoryAllocator::mem_alloc(size);
}

void Sem::operator delete(void *ptr) {
    MemoryAllocator::mem_free(ptr);
}

void *Sem::operator new[](size_t size) {

    size = ((size + MemoryAllocator::memSegSize) % MEM_BLOCK_SIZE == 0 ? (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE :
            (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE + 1);

    return MemoryAllocator::mem_alloc(size);
}

void Sem::operator delete[](void *ptr) {
    MemoryAllocator::mem_free(ptr);
}


