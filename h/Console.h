#include "../lib/hw.h"
class k_semaphore;
class k_Console{
public:
    static k_Console* getInstance();
    void rxPut(char t); //ulazni tok niti
    char rxGet();
    void txPut(char t);  //izlazni tok niti naseg OS
    char txGet();
    bool txEmpty();
private:
    k_Console();
    k_semaphore* rxSem;
    k_semaphore* txSem;
    static const int maxSize = 256;
    char rxBuffer[maxSize];
    char txBuffer[maxSize];
    int rxHead, rxTail;
    int txHead, txTail;
};