#include "../lib/hw.h"
enum Status{WAITING, RUNNING, SUSPENDED, READY, FINISHED};
class k_thread{
public:
    friend class Scheduler;
    static k_thread* running;
    void* operator new (size_t);
    void operator delete (void*); //our operators that are using class MemoryAllocator
    k_thread() = default;
    k_thread(void(*call)(void*), void* argument, void* alloc_space);
    static void dispatch();
    static void exit();
    void start();
    static void wrapper(); //when we dont have an context

private:
    size_t sp;
    k_thread *next;
    void* arg;
    void(*routine)(void*);
    void* memory;
    Status status = WAITING;
    void initContext();
};
extern "C" void yield(k_thread* current, k_thread* next);  //in ASM code