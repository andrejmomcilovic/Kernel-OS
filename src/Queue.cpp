#include "../h/Queue.h"
#include "../h/k_thread.h"
#include "../h/MemoryAllocator.h"

void* Queue :: operator new(size_t size){
    return MemoryAllocator::getInstance()->k_malloc(size);
}
void Queue :: operator delete(void* ptr){
    MemoryAllocator::getInstance()->k_free(ptr);
}

Queue :: Queue(){
    first = nullptr;
    last = nullptr;
}
void Queue::put(k_thread* curr){
    if(last){
        last->next = curr;
        curr->next = nullptr;  //for safe
        last = curr;
    }
    else{
        first = curr;
        last = curr;
        curr->next = nullptr;
    }
}
int Queue::getTokens() {
    if(first) return first->tokens;
    else return -1;
}
k_thread* Queue::get() {
    if((first == last) && first){
        k_thread* tmp = first;
        first = nullptr;
        last = nullptr;
        return tmp;
    }
    else if(first){
        k_thread* tmp = first;
        first = first->next;
        return tmp;
    }
    return nullptr;
}