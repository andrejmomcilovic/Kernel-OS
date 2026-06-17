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