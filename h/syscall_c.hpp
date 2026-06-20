#pragma once
#include "../lib/hw.h"
class k_thread;
typedef k_thread* thread_t;
extern "C" void* mem_alloc(size_t size);
extern "C" int mem_free (void*);
extern "C" int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
extern "C" int thread_exit ();
extern "C" void thread_dispatch ();