//Producer-Consumer using locks and condition variables

#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
struct Buffer{
    int max;
    std::vector<int> buff;
    int fill_ptr;
    int use_ptr;
    int count;

    std::mutex mtx;
    std::condition_variable fill, empty;
};

void init(Buffer* buffer, int capacity){
    buffer->max = capacity;
    buffer->buff.resize(buffer->max, 0);
    buffer->fill_ptr = 0;
    buffer->use_ptr = 0;
    buffer->count = 0;
}

void put(Buffer* buffer, int value){
    buffer->buff[buffer->fill_ptr] = value;
    buffer->fill_ptr = (buffer->fill_ptr + 1) % (buffer->max);
    buffer->count++;
}


int get(Buffer* buffer){
    int value = buffer->buff[buffer->use_ptr];
    buffer->use_ptr = (buffer->use_ptr + 1) % (buffer->max);
    buffer->count--;
    return value;
}

void producer(Buffer* buffer){

    for(int i = 0; i < buffer->max; i++){
        std::unique_lock<std::mutex> lock(buffer->mtx); //acquire the mutex (mtx)

        while(buffer->count == buffer->max)
        {
            buffer->empty.wait(lock); //the condition variable (empty i.e. the empty queue has this thread slept and the lock is released)
        }

        put(buffer, i);

        buffer->fill.notify_one(); //equivalent to signal code to notify one of the consumers
        //no unlocking required since out of scope means lock is released.
    }
}

void consumer(Buffer* buffer){

    for(int i = 0; i < buffer->max; i++){
        std::unique_lock<std::mutex> lock(buffer->mtx); //acquire the mutex (mtx)

        while(buffer->count == 0)
        {
            buffer->fill.wait(lock); //the condition variable (empty i.e. the empty queue has this thread slept and the lock is released)
        }

        int value = get(buffer);

        buffer->empty.notify_one(); //equivalent to signal code to notify one of the producers
        //no unlocking required since out of scope means lock is released.

        std::cout << "The value is "<< value <<std::endl;
    }
}

int main(){

    Buffer buffer;

    init(&buffer, 5);

    std::thread p1(producer, &buffer);
    std::thread c1(consumer, &buffer);

    p1.join();
    c1.join();

    return 0;
}

