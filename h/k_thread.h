#include "../lib/hw.h"
enum Status{WAITING, RUNNING, SUSPENDED, READY, FINISHED};
class k_thread{
public:
    friend class Scheduler;
    static k_thread* running;
    k_thread(void(*call)(void*), void* argument);
    static void dispatch();
    void exit();
    static void start();
    static void wrapper(); //when we dont have an context

private:
    k_thread *next;
    void* arg;
    void(*routine)(void*);
    void* memory;
    size_t sp;
    Status status = WAITING;
    void initContext();
};
extern "C" void yield(k_thread* current, k_thread* next);