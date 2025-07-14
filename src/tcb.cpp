//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"

TCB *TCB::running = nullptr;
int TCB::cnt = 1;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* arg, void* stackSpace)
{
    return new TCB(body, arg, stackSpace, TIME_SLICE);
}
int TCB::threadCreate(TCB** handle, void (*body)(void *), void *arg, void *stackSpace) {
    *handle =  createThread(body, arg, stackSpace);
    return 0;
}
int TCB::thread_exit() {
    if(running->isFinished()) return -1;
    else {
        running->setFinished(true);
        return 0;
    }
}

void TCB::yield()
{
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->blocked) {
        Scheduler::put(old);
    }
    running = Scheduler::get();
    Riscv::pushRegisters();
    TCB::contextSwitch(&old->context, &running->context);
    Riscv::popRegisters();
}

void TCB::threadWrapper()
{
    if(!running->sys)
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);  //switching to user mode
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}


void *TCB::operator new(size_t size) {
    size = ((size + MemoryAllocator::memSegSize) % MEM_BLOCK_SIZE == 0 ? (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE :
            (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE + 1);

    return MemoryAllocator::mem_alloc(size);
}

void TCB::operator delete(void *ptr) {
    MemoryAllocator::mem_free(ptr);
}

void *TCB::operator new[](size_t size) {

    size = ((size + MemoryAllocator::memSegSize) % MEM_BLOCK_SIZE == 0 ? (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE :
            (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE + 1);

    return MemoryAllocator::mem_alloc(size);
}

void TCB::operator delete[](void *ptr) {
    MemoryAllocator::mem_free(ptr);
}



