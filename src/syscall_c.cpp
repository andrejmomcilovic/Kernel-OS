#include "../h/syscall_c.hpp"
#include "../lib/hw.h"
// C API is wrapper to ABI and original Kernel code
void* mem_alloc(size_t size){
    size_t toAllocate = size / MEM_BLOCK_SIZE;
    if(size % MEM_BLOCK_SIZE) toAllocate++;  //size in blocks
    register uint64 a0 asm("a0") = 0x01;
    register uint64 a1 asm("a1") = toAllocate;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) : "memory");
    return (void*) a0;
}
int mem_free(void* addr){
    register uint64 a0 asm("a0") = 0x02;
    register uint64 a1 asm("a1") = (uint64)addr;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) : "memory");
    return (int)a0;
}
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    void* temp;
    temp = mem_alloc(DEFAULT_STACK_SIZE);
    register uint64 a0 asm("a0") = 0x11;
    register uint64 a1 asm("a1") = (uint64)handle;
    register uint64 a2 asm("a2") = (uint64)start_routine;
    register uint64 a3 asm("a3") = (uint64)arg;
    register uint64 a4 asm("a4") = (uint64)temp;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) , "r"(a2) , "r"(a3) , "r"(a4) : "memory");
    return (int)a0;
}
int thread_exit (){
    register uint64 a0 asm("a0") = 0x12;
    asm volatile("ecall" : "+r"(a0) : : "memory");
    return (int)a0;
}
void thread_dispatch (){
    register uint64 a0 asm("a0") = 0x13;
    asm volatile("ecall" : "+r"(a0) : : "memory");
}
int sem_open (sem_t* handle, unsigned init){
    register uint64 a0 asm("a0") = 0x21;
    register uint64 a1 asm("a1") = (uint64)handle;
    register uint64 a2 asm("a2") = (uint64)init;
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2) : "memory");
    return (int)a0;
}
int sem_close (sem_t handle){
    register uint64 a0 asm("a0") = 0x22;
    register uint64 a1 asm("a1") = (uint64)handle;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) : "memory");
    return (int)a0;
}
int sem_wait (sem_t id){
    register uint64 a0 asm("a0") = 0x23;
    register uint64 a1 asm("a1") = (uint64)id;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) : "memory");
    return (int)a0;
}
int sem_signal (sem_t id){
    register uint64 a0 asm("a0") = 0x24;
    register uint64 a1 asm("a1") = (uint64)id;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) : "memory");
    return (int)a0;
}
int sem_wait_n(sem_t id, unsigned n){
    register uint64 a0 asm("a0") = 0x25;
    register uint64 a1 asm("a1") = (uint64)id;
    register uint64 a2 asm("a2") = (uint64)n;
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2) : "memory");
    return (int)a0;
}
int sem_signal_n(sem_t id, unsigned n){
    register uint64 a0 asm("a0") = 0x26;
    register uint64 a1 asm("a1") = (uint64)id;
    register uint64 a2 asm("a2") = (uint64)n;
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2) : "memory");
    return (int)a0;
}
int time_sleep(time_t time){
    register uint64 a0 asm("a0") = 0x31;
    register uint64 a1 asm("a1") = (uint64)time;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) : "memory");
    return (int)a0;
}
char getc(){
    register uint64 a0 asm("a0") = 0x41;
    asm volatile("ecall" : "+r"(a0) : : "memory");
    return (char)a0;
}
void putc(char t){
    register uint64 a0 asm("a0") = 0x42;
    register uint64 a1 asm("a1") = (uint64)t;
    asm volatile("ecall" : "+r"(a0) : "r"(a1) : "memory");
}