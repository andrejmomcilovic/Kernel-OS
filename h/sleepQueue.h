#include "../lib/hw.h"
class k_thread;
class sleepQueue{
public:
    static sleepQueue* getInstance();
    void insert(k_thread *curr, time_t time);
    void update();
    k_thread* popExpired();
private:
    k_thread *first;
    sleepQueue();
};