#pragma once
#include "../lib/hw.h"
class k_thread;
typedef k_thread* thread_t;
class k_semaphore;
typedef k_semaphore* sem_t;
extern "C" void* mem_alloc(size_t size);
extern "C" int mem_free (void*);
extern "C" int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
extern "C" int thread_exit ();
extern "C" void thread_dispatch ();
extern "C" int sem_open (sem_t* handle, unsigned init);
extern "C" int sem_close (sem_t handle);
extern "C" int sem_wait (sem_t id);
extern "C" int sem_signal (sem_t id);
extern "C" int sem_wait_n(sem_t id, unsigned n);
extern "C" int sem_signal_n(sem_t id, unsigned n);