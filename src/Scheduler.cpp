#include "../h/Scheduler.h"
#include "../h/k_thread.h"

Scheduler* Scheduler::getInstance() {
    static Scheduler instance;
    return &instance;
}
void Scheduler::put(k_thread* curr){
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
k_thread* Scheduler::get() {
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