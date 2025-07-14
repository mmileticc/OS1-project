//
// Created by os on 6/22/24.
//
#include "../h/syscall_cpp.hpp"

void *operator new(size_t s) {
    return mem_alloc(s);
}

void *operator new[](size_t s) {
    return mem_alloc(s);
}

void operator delete(void *p) noexcept{
    mem_free(p);
}
void operator delete[](void *p) noexcept{
    mem_free(p);
}


