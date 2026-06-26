#include "../h/sleepQueue.h"
#include "../h/k_thread.h"

sleepQueue* sleepQueue ::getInstance() {
    static sleepQueue instance;
    return &instance;
}
sleepQueue ::sleepQueue() {
    first = nullptr;
}

void sleepQueue :: insert(k_thread *curr, time_t time){
    if(!first){
        first = curr;
        curr->sleepTime = time;
        curr->nextSleep = nullptr;
        return;
    }

    if (time < first->sleepTime) {
        first->sleepTime -= time;
        curr->nextSleep = first;
        curr->sleepTime = time;
        first = curr;
        return;
    }

    k_thread* begin = first;
    k_thread* prev = nullptr;


    while (begin && time >= begin->sleepTime) {
        time -= begin->sleepTime;
        prev = begin;
        begin = begin->nextSleep;
    }


    curr->sleepTime = time;
    curr->nextSleep = begin;
    prev->nextSleep = curr;

    if (begin) {
        begin->sleepTime -= curr->sleepTime;
    }
}
void sleepQueue :: update(){
    if(first){
        first->sleepTime--;
    }
}
k_thread* sleepQueue ::popExpired() {
    if(first && first->sleepTime == 0){
        k_thread* curr = first;
        first = first->nextSleep;
        curr->nextSleep = nullptr;
        return curr;
    }
    return nullptr;
}