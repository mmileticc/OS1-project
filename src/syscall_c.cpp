//
// Created by os on 6/22/24.
//
#include  "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"


void* mem_alloc (size_t size){
    size_t  s = size + MemoryAllocator::memSegSize;
    size = (size + MemoryAllocator::memSegSize) / MEM_BLOCK_SIZE;
    if(s % MEM_BLOCK_SIZE != 0) size += 1;

    Riscv::sysCallWraper(Riscv::memAlloc, size);
    uint64 ret = Riscv::r_a0();
    return (void*)ret;
}

int mem_free(void *p){
    Riscv::sysCallWraper(Riscv::memFree, p);
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" : [ret] "=r" (ret));
    return (int) ret;
}

int thread_create ( thread_t* handle, void(*body)(void*), void* arg) {  //0x11
    void* stack = mem_alloc( DEFAULT_STACK_SIZE);
    if(stack == nullptr) return -1;
    Riscv::sysCallWraper(Riscv::threadCreate, handle, body, arg, stack);
    uint64 ret = Riscv::r_a0();
    return (int) ret;
}

int thread_exit (){  //0x12
    Riscv::sysCallWraper(Riscv::threadExit);
    uint64 ret = Riscv::r_a0();
    return (int) ret;
}

void thread_dispatch (){  //0x13
    Riscv::sysCallWraper(Riscv::threadDispatch);
}

int sem_open (sem_t* handle, unsigned init){//0x21
    Riscv::sysCallWraper(Riscv::semOpen, handle, init);
    uint64 ret = Riscv::r_a0();
    return (int) ret;
}
int sem_close (sem_t handle){ //0x22
    Riscv::sysCallWraper(Riscv::semClose, handle);
    uint64 ret = Riscv::r_a0();
    return (int) ret;
}
int sem_wait (sem_t id){ //0x23
    Riscv::sysCallWraper(Riscv::semWait, id);
    uint64 ret = Riscv::r_a0();
    return (int) ret;
}
int sem_signal (sem_t id){  //0x24
    Riscv::sysCallWraper(Riscv::semSignal, id);
    uint64 ret = Riscv::r_a0();
    return (int) ret;
}
int sem_timedwait(sem_t id,time_t timeout){return 0;} //0x25      //////////////////

int sem_trywait(sem_t id){ //0x26
    Riscv::sysCallWraper(Riscv::semTrywait, id);
    uint64 ret = Riscv::r_a0();
    return (int) ret;
}
typedef unsigned long time_t;
int time_sleep (time_t){ return 0;}  //0x31                      //////////////////

char getc() {  //0x41                                           //////////////////
    Riscv::sysCallWraper(Riscv::getC);
    uint64 ret = Riscv::r_a0();
    return (char) ret;
}

void putc(char c) {  //0x42                                     //////////////////
    Riscv::sysCallWraper(Riscv::putC, c);
}

