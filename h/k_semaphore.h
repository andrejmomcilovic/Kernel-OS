#pragma once
#include "../lib/hw.h"
class Queue;
class k_semaphore{
public:
    k_semaphore(unsigned  init);
    void* operator new (size_t);
    void operator delete (void*);
    int wait();
    int signal();
    int sem_close();
    int sem_wait_n(unsigned n);
    int sem_signal_n(unsigned n);
private:
    int val;
    Queue* blocked;
};