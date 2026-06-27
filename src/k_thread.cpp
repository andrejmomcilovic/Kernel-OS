#include "../h/k_thread.h"
#include "../h/Scheduler.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"

k_thread* volatile k_thread::running = nullptr;
k_thread* k_thread::mainThread = nullptr;
volatile int k_thread :: count = 1;
void k_thread::start(){
    Scheduler::getInstance()->put(this);
}

void* k_thread::operator new(size_t size){
    return MemoryAllocator::getInstance()->k_malloc(size);
}

void k_thread::operator delete(void* ptr){
    MemoryAllocator::getInstance()->k_free(ptr);
}

k_thread::k_thread(void(*call)(void*), void* argument, void* alloc_space, bool user){
    currTime = 0;
    arg = argument;
    isUserThread = user;
    routine = call;
    memory = alloc_space;
    sp = (size_t)((char*)memory + DEFAULT_STACK_SIZE);
    initContext();
}

void k_thread::initContext() {
    size_t* context = (size_t*)sp - 28;
    context[15] = (size_t)&wrapper;
    context[28] = (size_t)&wrapper;
    sp = (size_t)context;
}
extern "C" void threadExit(){
    thread_exit();
}
void k_thread::wrapper(){
    //asm volatile("csrs sstatus, %0" : : "r"(2));
    // kernel nit
    if(running->isUserThread){
        uint64 sppVal;
        asm volatile("csrr %0, sstatus" : "=r"(sppVal));
        sppVal &= ~256UL;    // SPP=0
        sppVal |= 32UL;      // SPIE=1
        asm volatile("csrw sstatus, %0" : : "r"(sppVal));
        asm volatile("csrw sepc, %0" : : "r"(running->routine));
        asm volatile("mv a0, %0" : : "r"(running->arg));
        asm volatile("la ra, threadExit");
        asm volatile("sret");
    }
    asm volatile("la ra, threadExit");  //we want to exit thread
    running->routine(running->arg);
    thread_exit();
}

void k_thread::dispatch() {
    Scheduler::getInstance()->put(running);
    k_thread* next = Scheduler::getInstance()->get();
    k_thread* curr = running;
    running = next;
    if(!running->isUserThread){
        uint64 sppVal;
        asm volatile("csrr %0, sstatus" : "=r"(sppVal));
        sppVal |= 256UL;     // SPP=1
        sppVal |= 32UL;      // SPIE=1
        asm volatile("csrw sstatus, %0" : : "r"(sppVal));
    }
    else{
        uint64 sppVal;
        asm volatile("csrr %0, sstatus" : "=r"(sppVal));
        sppVal &= ~256UL;    // SPP=0
        sppVal |= 32UL;      // SPIE=1
        asm volatile("csrw sstatus, %0" : : "r"(sppVal));
    }
    yield(curr, next);
}
void k_thread::exit() {
    k_thread* next = Scheduler::getInstance()->get();
    if(next == nullptr) next = mainThread;
    k_thread* curr = running;
    running = next;
    curr->status = FINISHED;
    if(!running->isUserThread){
        uint64 sppVal;
        asm volatile("csrr %0, sstatus" : "=r"(sppVal));
        sppVal |= 256UL;     // SPP=1
        sppVal |= 32UL;      // SPIE=1
        asm volatile("csrw sstatus, %0" : : "r"(sppVal));
    }
    else{
        uint64 sppVal;
        asm volatile("csrr %0, sstatus" : "=r"(sppVal));
        sppVal &= ~256UL;    // SPP=0
        sppVal |= 32UL;      // SPIE=1
        asm volatile("csrw sstatus, %0" : : "r"(sppVal));
    }
    yield(curr, next);
}