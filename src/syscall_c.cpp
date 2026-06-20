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