#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/k_thread.h"
#include "../h/syscall_c.hpp"
#include "../h/k_semaphore.h"
#include "../lib/hw.h"

static void halt() { *(volatile unsigned int*)0x100000 = 0x5555; }

sem_t sem;

extern void userMain();

extern "C" bool interruptHandler(){
    bool result;
    uint64 regValue;
    asm volatile("csrr %0, scause" : "=r"(regValue));
    if(regValue == 0x8000000000000001){
        uint64 sipVal;
        asm volatile("csrr %0, sip" : "=r"(sipVal));
        sipVal &= ~2UL;
        asm volatile("csrw sip, %0" : : "r"(sipVal));
        k_thread *curr = k_thread::running;
        curr->currTime++;
        if(curr->currTime == DEFAULT_TIME_SLICE){
            curr->currTime = 0;
            k_thread::dispatch();
        }
        result = true;
    }
    else if(regValue == 0x8000000000000009){
        int irq = plic_claim();
        console_handler();
        plic_complete(irq);
        result = true;
    }
    else if(regValue == 2){
        result = true;
    }
    else{
        result = false;
    }
    if(!k_thread::running->isUserThread){
        uint64 sppVal;
        asm volatile("csrr %0, sstatus" : "=r"(sppVal));
        sppVal |= 256UL;
        sppVal |= 32UL;
        asm volatile("csrw sstatus, %0" : : "r"(sppVal));
    }
    else{
        uint64 sppVal;
        asm volatile("csrr %0, sstatus" : "=r"(sppVal));
        sppVal &= ~256UL;
        sppVal |= 32UL;
        asm volatile("csrw sstatus, %0" : : "r"(sppVal));
    }
    return result;
}

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
            k_thread *t = new k_thread((void(*)(void*))a2, (void*)a3, (void*)a4);
            t->start();
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
        case 0x21:{
            k_semaphore *sem = new k_semaphore((unsigned)a2);
            if(sem == nullptr){ value = -1; break; }
            k_semaphore **tmp = (k_semaphore**)a1;
            *tmp = sem;
            value = 0;
            break;
        }
        case 0x22:{
            k_semaphore *sem = (k_semaphore*) a1;
            value = sem->sem_close();
            break;
        }
        case 0x23:{
            k_semaphore *sem = (k_semaphore*) a1;
            value = sem->wait();
            break;
        }
        case 0x24:{
            k_semaphore *sem = (k_semaphore*) a1;
            value = sem->signal();
            break;
        }
        case 0x25:{
            k_semaphore *sem = (k_semaphore*) a1;
            unsigned val = (unsigned)a2;
            value = sem->sem_wait_n(val);
            break;
        }
        case 0x26:{
            k_semaphore *sem = (k_semaphore*) a1;
            unsigned val = (unsigned)a2;
            value = sem->sem_signal_n(val);
            break;
        }
    }
    return value;
}

sem_t mutex;
int counter = 0;

void taskA(void* arg){
    while(true){
        sem_wait(mutex);
        counter++;
        __putc('A');
        __putc('0' + counter % 10);
        sem_signal(mutex);
        thread_dispatch();
    }
}

void taskB(void* arg){
    while(true){
        sem_wait(mutex);
        counter++;
        __putc('B');
        __putc('0' + counter % 10);
        sem_signal(mutex);
        thread_dispatch();
    }
}

void userMain(){
    sem_open(&mutex, 1);
    thread_t hA, hB;
    thread_create(&hA, taskA, nullptr);
    thread_create(&hB, taskB, nullptr);
    while(true){ thread_dispatch(); }
}

void userWrapper(void* ptr){
    userMain();
}

void main() {
    asm volatile("la t0, InterruptRoutine");
    asm volatile("csrw stvec, t0");
    k_thread* main_Thread = new k_thread();
    k_thread::mainThread = main_Thread;
    void* allocspace = MemoryAllocator::getInstance()->k_malloc(DEFAULT_STACK_SIZE);
    k_thread* userThread = new k_thread(userWrapper, nullptr, allocspace);
    k_thread::running = main_Thread;
    asm volatile("csrs sstatus, %0" : : "r"(2));
    asm volatile("csrs sie, %0" : : "r"(2)); //odluta posle ove linija kada se doda value == 2
    k_thread::running = userThread;
    yield(main_Thread, userThread);
    halt();
}