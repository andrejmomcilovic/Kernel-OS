#include "../h/Console.h"
#include "../h/k_semaphore.h"

k_Console :: k_Console(){
    rxSem = new k_semaphore(0);
    txSem = new k_semaphore(0);
    rxHead = 0;
    rxTail = 0;
    txHead = 0;
    txTail = 0;
}
k_Console* k_Console ::getInstance() {
    static k_Console instance;
    return &instance;
}
void k_Console :: rxPut(char t){
    rxBuffer[rxTail  % maxSize] = t;
    rxTail = (rxTail + 1) % maxSize;
    rxSem->signal();
}
char k_Console :: rxGet(){
    rxSem->wait();
    char ret = rxBuffer[rxHead];
    rxHead = (rxHead + 1) % maxSize;
    return ret;
}

void k_Console :: txPut(char t){
    txBuffer[txTail  % maxSize] = t;
    txTail = (txTail + 1) % maxSize;
    txSem->signal();
}
char k_Console :: txGet(){
    txSem->wait();
    char ret = txBuffer[txHead];
    txHead = (txHead + 1) % maxSize;
    return ret;
}
bool k_Console :: txEmpty(){
    return txHead == txTail;
}

