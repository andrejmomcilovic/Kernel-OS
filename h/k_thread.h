#pragma once
#include "../lib/hw.h"
enum Status{WAITING, RUNNING, SUSPENDED, READY, FINISHED};
extern "C" bool interruptHandler();
class k_thread{
public:
    static k_thread* mainThread; //to switch context here
    friend class Scheduler;
    friend class Queue;
    friend class k_semaphore;
    static k_thread* volatile running;
    void* operator new (size_t);
    void operator delete (void*); //our operators that are using class MemoryAllocator
    k_thread() = default;
    k_thread(void(*call)(void*), void* argument, void* alloc_space, bool user = true);
    static void dispatch();
    static void exit();
    void start();
    static void wrapper(); //when we dont have an context
    friend bool interruptHandler();

private:
    size_t sp;
    k_thread *next;
    size_t currTime;
    void* arg;
    void(*routine)(void*);
    void* memory;
    Status status = WAITING;
    int tokens;
    void initContext();
public:
    bool isUserThread = false;
};
extern "C" void yield(k_thread* current, k_thread* next);  //in ASM code