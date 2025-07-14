//
// Created by os on 6/26/24.
//
#include "../h/syscall_cpp.hpp"
#include "../h/Sem.hpp"

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    //sem_close(myHandle);
    //delete (Sem*)myHandle;
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t) {
    return 0;
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}
