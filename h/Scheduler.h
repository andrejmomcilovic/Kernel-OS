#pragma once
class k_thread;
class Scheduler{
public:
    static Scheduler* getInstance();
    void put(k_thread* curr);
    k_thread* get();
private:
    Scheduler();
    k_thread* first;
    k_thread* last;
};