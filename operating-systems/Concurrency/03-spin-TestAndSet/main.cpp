#include <iostream>
#include <atomic>
#include <thread>
struct spinLock{
    std::atomic<bool> flag;
};

struct Counter{
    int value;
    spinLock lock;
};

void init(spinLock* lock){
    lock->flag = false;
}

void init(Counter* counter){
    counter->value = 0;
    init(&counter->lock);
}

void acquire(spinLock* lock){
    while(lock->flag.exchange(true)){
        //do-nothing (spin)
    }
}

void release(spinLock* lock){
    lock->flag = false;
}

void increment(Counter* c){
    acquire(&c->lock);
        c->value++;
    release(&c->lock);
}

void worker(Counter* c){
    for(int i = 0; i < 100000; i++){
        increment(c);
    }
}
int main(){
    Counter counter;
    init(&counter);

    std::thread t1(worker, &counter);
    std::thread t2(worker, &counter);

    t1.join();
    t2.join();

    std::cout <<"Counter value is "<<counter.value << std::endl;
    
    return 0;
}