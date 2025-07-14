//
// Created by os on 6/26/24.
//
#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

void threadStartWrapper(void *arg) {
    Thread *thread = (Thread *) arg;
    thread->run();
    /*if (thread->body != nullptr) {
        thread->body(thread->arg);
    } else {
        thread->run();
    }*/
}

Thread::Thread (void (*body)(void*), void* arg):myHandle(nullptr), body(body), arg(arg){}

Thread::~Thread () {
   /* TCB* h = (TCB*)myHandle;
    delete h;*/
}
/*
int Thread::start (){
    return thread_create(&myHandle, &threadStartWrapper, (void*)this);
}*/

int Thread::start() {
    if (this->body) return thread_create(&this->myHandle, this->body, this->arg);
    else return thread_create(&this->myHandle, threadStartWrapper,this);
}
void Thread::dispatch (){
    thread_dispatch();
}
int Thread::sleep(time_t) {
    return 0;
}

Thread::Thread():myHandle(nullptr), body(nullptr), arg(nullptr) {}

