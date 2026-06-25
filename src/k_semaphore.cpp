#include "../h/k_semaphore.h"
#include "../h/Queue.h"
#include "../h/k_thread.h"
#include "../h/Scheduler.h"
#include "../h/MemoryAllocator.h"
void* k_semaphore :: operator new(size_t size){
    return MemoryAllocator::getInstance()->k_malloc(size);
}
void k_semaphore :: operator delete(void* ptr){
    MemoryAllocator::getInstance()->k_free(ptr);
}
k_semaphore :: k_semaphore(unsigned init){
    val = init;
    blocked = new Queue();
}
int k_semaphore :: wait(){
    if(val > 0) val--;
    else{
        k_thread* curr = k_thread::running;
        curr->tokens = 1;
        curr->status = SUSPENDED;
        blocked->put(curr);
        k_thread* next = Scheduler::getInstance()->get();
        if(next == nullptr) next = k_thread::mainThread;
        k_thread::running = next;
        if(!k_thread::running->isUserThread){
            uint64 sppVal;
            asm volatile("csrr %0, sstatus" : "=r"(sppVal));
            sppVal |= 256UL;
            sppVal |= 32UL;
            asm volatile("csrw sstatus, %0" : : "r"(sppVal));
        }
        else{
            uint64 sppVal;
            asm volatile("csrr %0, sstatus" : "=r"(sppVal));
            sppVal &= ~256UL;
            sppVal |= 32UL;
            asm volatile("csrw sstatus, %0" : : "r"(sppVal));
        }
        yield(curr, next);
    }
    return 0;
}
int k_semaphore :: signal(){
    val++;
    while(blocked->getTokens() != -1 && val){
        if(blocked->getTokens() <= val){
            val -= blocked->getTokens();
            k_thread* next = blocked->get();
            next->status = READY;
            Scheduler::getInstance()->put(next);
        }
        else break;
    }
    return 0;
}
int k_semaphore :: sem_close(){
    k_thread* toPut;
    while((toPut = blocked->get())){
        toPut->status = READY;
        Scheduler::getInstance()->put(toPut);
    }
    val = 0;
    return 0;
}
int k_semaphore ::sem_wait_n(unsigned n) {
    if(val >= (int)n) val -= n;
    else{
        k_thread* curr = k_thread::running;
        curr->tokens = n;
        curr->status = SUSPENDED;
        blocked->put(curr);
        k_thread* next = Scheduler::getInstance()->get();
        if(next == nullptr) next = k_thread::mainThread;
        k_thread::running = next;
        if(!k_thread::running->isUserThread){
            uint64 sppVal;
            asm volatile("csrr %0, sstatus" : "=r"(sppVal));
            sppVal |= 256UL;
            sppVal |= 32UL;
            asm volatile("csrw sstatus, %0" : : "r"(sppVal));
        }
        else{
            uint64 sppVal;
            asm volatile("csrr %0, sstatus" : "=r"(sppVal));
            sppVal &= ~256UL;
            sppVal |= 32UL;
            asm volatile("csrw sstatus, %0" : : "r"(sppVal));
        }
        yield(curr, next);
    }
    return 0;
}
int k_semaphore ::sem_signal_n(unsigned n) {
    val += n;
    while(blocked->getTokens() != -1 && val){
        if(blocked->getTokens() <= val){
            val -= blocked->getTokens();
            k_thread* next = blocked->get();
            next->status = READY;
            Scheduler::getInstance()->put(next);
        }
        else break;
    }
    return 0;
}