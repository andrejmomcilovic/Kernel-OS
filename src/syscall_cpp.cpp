/*#include "../h/syscall_cpp.hpp"
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
    thread_create(&myHandle, body, arg);
}
void Thread :: dispatch(){
    thread_dispatch();
}
Thread :: Thread(){

}
Semaphore :: Semaphore(unsigned init = 1){
    sem_open(&myHandle, init);
}
int Semaphore :: wait(){
    return sem_wait(myHandle);
}
int Semaphore :: signal(){
    return sem_signal(myHandle);
}*/


