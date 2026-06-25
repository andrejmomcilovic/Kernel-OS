#pragma once
#include "../lib/hw.h"
class k_thread;
class Queue{
public:
    Queue();
    void* operator new (size_t);
    void operator delete (void*);
    void put(k_thread* curr);
    int getTokens();
    k_thread* get();
private:
    k_thread* first;
    k_thread* last;
};