#pragma once
#include "../lib/hw.h"

struct MemBlock{
    size_t size;
    MemBlock* next;
};

class MemoryAllocator{
public:
    static MemoryAllocator* getInstance();
    void* k_malloc(size_t bytes);
    int k_free(void* addr);
private:
    MemoryAllocator();
    MemBlock* begin;   //begin block in node list
};