#include <iostream>
#include <thread>
#include <mutex>

struct Counter{
    int value;
    std::mutex lock;
};

void init(Counter* c){
    c->value = 0;
}

void increment(Counter* c){
    // c->lock.lock();
    std::lock_guard<std::mutex> guard(c->lock);
    //this is a prptection measure, in the CS below, if exception occurs, function returns and the destroyer is called
    //releasing (unlocking) the lock itself, so the lock isn't held for eternity
    c->value++;
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
    //both threads are modifying the same object;

    t1.join();
    t2.join();

    std::cout <<"Counter value is "<<counter.value << std::endl;
    // OUPUT without lock - Counter value is 166486 (so it is not 200000, so race condition occured)
    // OUTPUT with lock - Counter value is 200000 
    return 0;
}