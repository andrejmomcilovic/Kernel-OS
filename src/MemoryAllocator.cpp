#include "../h/MemoryAllocator.h"

MemoryAllocator :: MemoryAllocator(){
    begin = (MemBlock*)HEAP_START_ADDR;
    begin->size = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR;
    begin->next = nullptr;
}

void* MemoryAllocator :: k_malloc(size_t bytes){
    MemBlock* prev = nullptr;
    MemBlock* curr;
    bool found = false;
    for(curr = begin; curr; curr = curr->next){
        if(curr->size >= bytes + sizeof(MemBlock)){
            found = true;
            break;
        }
        prev = curr;
    }
    if(prev && found){
        //da li nam ostaje dovoljno ili imamo fragmentaciju
        //char* addr = curr + bytes + sizeof(MemBlock);
        if(curr->size >=  bytes + sizeof(MemBlock) + sizeof(MemBlock) + MEM_BLOCK_SIZE){
              MemBlock* block = (MemBlock*)((char*)curr + bytes + sizeof(MemBlock));
              block->size = curr->size - sizeof(MemBlock) - bytes;
              block->next = curr->next;
              prev->next = block;
              curr->size = bytes + sizeof(MemBlock);
        }
        else{
            prev->next = curr->next;
        }
        return (char*)curr + sizeof(MemBlock);
    }
    else if(!prev && found) {
        if (curr->size >= bytes + sizeof(MemBlock) + sizeof(MemBlock) + MEM_BLOCK_SIZE) {
            MemBlock *block = (MemBlock *) ((char *) curr + bytes + sizeof(MemBlock));
            block->size = curr->size - sizeof(MemBlock) - bytes;
            block->next = curr->next;
            begin = block;
            curr->size = bytes + sizeof(MemBlock);
        }
        else {
            begin = curr->next;
        }
        return (char*)curr + sizeof(MemBlock);
    }
    return nullptr;
}

int MemoryAllocator :: k_free(void* addr){
    if(addr == nullptr) return -1;
    char* start = (char*)addr - sizeof(MemBlock);
    if((char*)HEAP_START_ADDR > start || (char*)HEAP_END_ADDR<= start) return -2;
    MemBlock* curr;
    MemBlock* downBound = nullptr;
    MemBlock* upperBound = nullptr;
    for(curr = begin; curr; curr = curr->next){
        if(curr < (MemBlock*)start) downBound = curr;
        if(curr > (MemBlock*)start){
            upperBound = curr;
            break;
        }
    }
    MemBlock* tmp = (MemBlock*)start;
    if(downBound && upperBound){
        downBound->next = tmp;
        tmp->next = upperBound;
        if(start + tmp->size == (char*)upperBound){
            tmp->size += upperBound->size;
            tmp->next = upperBound->next;
        }
        if((char*)downBound + downBound->size == start){
            downBound->size += tmp->size;
            downBound->next = tmp->next;
        }
    }
    else if(!downBound && upperBound){
        tmp->next = upperBound;
        if(start + tmp->size == (char*)upperBound){
            tmp->size += upperBound->size;
            tmp->next = upperBound->next;
        }
        begin = tmp;
    }
    else if(downBound && !upperBound){
        tmp->next = downBound->next;
        downBound->next = tmp;
        if((char*)downBound + downBound->size == start){
            downBound->size += tmp->size;
            downBound->next = tmp->next;
        }
    }
    else{
        begin = tmp;
        tmp->next = nullptr;
    }
    return 0;
}


MemoryAllocator* MemoryAllocator:: getInstance() {
    static MemoryAllocator instance;  //komplajer pravi staticki alocitan objekat u memoriju
    return &instance;
}