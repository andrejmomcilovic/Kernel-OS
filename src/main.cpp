#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/k_thread.h"
//static void print(const char* s) { while (*s) __putc(*s++); }
static void halt() { *(volatile unsigned int*)0x100000 = 0x5555; }

extern "C" uint64 syscall(uint64 code, uint64 a1, uint64 a2, uint64 a3, uint64 a4){
    uint64 value = -1;
    switch(code){
        case 0x01:{
            value = (uint64)(MemoryAllocator::getInstance()->k_malloc(a1 * MEM_BLOCK_SIZE));
            break;
        }
        case 0x02:{
            value = ((uint64)MemoryAllocator::getInstance()->k_free((void*)a1));
            break;
        }
        case 0x11:{
            k_thread *t = new k_thread((void(*)(void*))a2, (void*)a3, (void*)a4); //cast za a2
            k_thread **temp = (k_thread**)a1;
            *temp = t;
            if(t != nullptr) value = 0;
            else value = -1;
            break;
        }
        case 0x12:{
            k_thread::exit();
            value = 0;
            break;
        }
        case 0x13:{
            k_thread::dispatch();
            value = 0;
            break;
        }

    }
    return value;
}


void task1(void* ptr){
    while(true){
        __putc('A');
        k_thread::dispatch();
    }
}
void task2(void* ptr){
    while(true){
        __putc('B');
        k_thread::dispatch();
    }
}

void userMain(){
    k_thread *thread1 = new k_thread(task1, nullptr);
    k_thread *thread2 = new k_thread(task2, nullptr);
    thread1->start();
    thread2->start();
    __putc('U');
    k_thread::dispatch();
}

void userWrapper(void* ptr){
    userMain();
}

void main() {
    // postavi stvec (prekidna rutina) - mora pre ecall-a
    asm volatile("la t0, InterruptRoutine");
    asm volatile("csrw stvec, t0");
    k_thread* mainThread = new k_thread();
    k_thread* userThread = new k_thread(userWrapper, nullptr);
    k_thread::running = userThread;
    yield(mainThread, userThread);
    halt();
}