#include "../lib/console.h"
#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.h"
static void print(const char* s) { while (*s) __putc(*s++); }
static void halt() { *(volatile unsigned int*)0x100000 = 0x5555; }

extern "C" uint64 syscall(uint64 code, uint64 a1, uint64 a2){
    uint64 value = -1;
    switch(code){
        case 0x01:
            value = (uint64)(MemoryAllocator::getInstance()->k_malloc(a1 * MEM_BLOCK_SIZE));
            break;
        case 0x02:
            value = ((uint64)MemoryAllocator::getInstance()->k_free((void*)a1));
            break;
    }
    return value;
}

int main() {
    // postavi stvec (prekidna rutina) - mora pre ecall-a
    asm volatile("la t0, InterruptRoutine");
    asm volatile("csrw stvec, t0");

    // 1. alokacija kroz ecall vraca validan pokazivac
    void* p1 = mem_alloc(100);
    print(p1 ? "1 OK\n" : "1 PAO\n");

    // 2. druga alokacija, razlicita i iza prve
    void* p2 = mem_alloc(100);
    print((p2 && p2 > p1) ? "2 OK\n" : "2 PAO\n");

    // 3. free vraca 0 (uspeh)
    int r = mem_free(p1);
    print(r == 0 ? "3 OK\n" : "3 PAO\n");

    // 4. realokacija iste velicine uzima oslobodjenu rupu
    void* p3 = mem_alloc(100);
    print(p3 == p1 ? "4 OK\n" : "4 PAO\n");

    // 5. coalesce kroz ceo lanac: oslobodi sve, trazi veci blok
    mem_free(p2);
    mem_free(p3);
    void* big = mem_alloc(500);
    print(big ? "5 OK\n" : "5 PAO\n");

    print("--- kraj ---\n");
    halt();
    return 0;
}