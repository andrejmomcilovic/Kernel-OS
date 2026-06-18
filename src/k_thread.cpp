#include "../h/k_thread.h"
#include "../h/Scheduler.h"
#include "../h/MemoryAllocator.h"
void k_thread :: start(){
    Scheduler::getInstance()->put(this);
}
k_thread :: k_thread(void(*call)(void*), void* argument){
    arg = argument;
    routine = call;
    memory = MemoryAllocator::getInstance()->k_malloc(DEFAULT_STACK_SIZE);  //potentionally nullptr
    sp = (size_t)((char*)memory + DEFAULT_STACK_SIZE);
    initContext();
}
void k_thread ::initContext() {
    size_t* context = (size_t*)sp - 28;
    context[15] = (size_t)&wrapper;  //upisujemo funkciju u deo za ra
    sp = (size_t)context;
}

void k_thread :: wrapper(){
    running->routine(running->arg);
    exit();
}

void k_thread :: dispatch() {
    Scheduler::getInstance()->put(running);
    k_thread* next = Scheduler::getInstance()->get();
    k_thread* curr = running;
    running = next;
    yield(curr, next);
}

void k_thread :: exit() {
    k_thread* next = Scheduler::getInstance()->get();
    k_thread* curr = running;
    running = next;
    curr->status = FINISHED;
    yield(curr, next);
}