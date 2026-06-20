class Queue;
class k_semaphore{
public:
    k_semaphore(unsigned  init);
    void* operator new (size_t);
    void operator delete (void*);
    int wait();
    int signal();
    int sem_close();
    int sem_wait_n(int n);
    int sem_signal_n(int n);
private:
    int val;
    Queue* blocked;
};