#include "../h/syscall_cpp.hpp"
void* operator new(size_t size){
    return mem_alloc(size);
}
void operator delete(void* sth){
    mem_free(sth);
}
Thread :: Thread(void (*body)(void*), void* arg){
    this->body = body;
    this->arg = arg;
}
int Thread :: start(){
    return thread_create(&myHandle, body, arg);
}
void Thread :: dispatch(){
    thread_dispatch();
}
int Thread :: sleep(time_t time){
    return time_sleep(time);
}
Thread :: Thread(){};

Semaphore :: Semaphore(unsigned init){
    sem_open(&myHandle, init);
}
int Semaphore :: wait(){
    return sem_wait(myHandle);
}
int Semaphore :: signal(){
    return sem_signal(myHandle);
}
Semaphore :: ~Semaphore(){
    sem_close(myHandle);
}
char Console :: getc(){
    return ::getc();
}
void Console :: putc(char t){
    ::putc(t);
}




