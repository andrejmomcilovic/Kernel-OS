#include "../h/k_semaphore.h"
#include "../h/Queue.h"
#include "../h/k_thread.h"
#include "../h/Scheduler.h"
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
        k_thread::running = next;
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
    while(toPut = blocked->get()){
        toPut->status = READY;
        Scheduler::getInstance()->put(toPut);
    }
    val = 0;
    return 0;
}
int k_semaphore ::sem_wait_n(int n) {
    if(val >= n) val -= n;
    else{
        k_thread* curr = k_thread::running;
        curr->tokens = n;
        curr->status = SUSPENDED;
        blocked->put(curr);
        k_thread* next = Scheduler::getInstance()->get();
        k_thread::running = next;
        yield(curr, next);
    }
    return 0;
}
int k_semaphore ::sem_signal_n(int n) {
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