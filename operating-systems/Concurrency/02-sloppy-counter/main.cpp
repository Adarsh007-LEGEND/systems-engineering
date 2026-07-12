#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

struct SloppyCounter{
    int global;

    std::mutex global_lock;
    std::vector<int> local; //local counter per cpu
    // std::vector<std::mutex> local_locks; this cant be done, because vector constructs mutex objects internally and it likes types that can be moved
    // or copied when it grows, but mutexes cannot be copied in c++
    std::mutex local_locks[4];

    int threshold;
};

//initialize
void init(SloppyCounter* counter, int threshold, int numCpus){
    counter->global = 0;
    counter->threshold = threshold;
    counter->local.resize(numCpus);
    // counter->local_locks.resize(numCpus);

    for(int i = 0; i < numCpus; i++){
        counter->local[i] = 0;
    }
}

//update
void update(SloppyCounter* counter, int numCpus, int threadId, int amt){
    int cpu = threadId % numCpus;
    std::lock_guard<std::mutex> guard(counter->local_locks[cpu]);
    counter->local[cpu] += amt;

    if(counter->local[cpu] >= counter->threshold){
        std::lock_guard<std::mutex> guard(counter->global_lock);
        counter->global += counter->local[cpu];
        counter->local[cpu] = 0;
    }
}

//get
int get(SloppyCounter* counter){
    std::lock_guard<std::mutex> guard(counter->global_lock);
    int val = counter->global;
    return val;
}

void worker(SloppyCounter* counter, int numCpus, int threadId)
{
    for(int i = 0; i < 100003; i++)
    {
        update(counter, numCpus, threadId, 1);
    }
}

int main()
{
    const int NUM_CPUS = 4;
    const int NUM_THREADS = 8;
    const int THRESHOLD = 100;

    SloppyCounter counter;

    init(&counter, THRESHOLD, NUM_CPUS);

    // create 8 threads
    std::vector<std::thread> threads;
    for(int i = 0; i < NUM_THREADS; i++){
        threads.emplace_back(worker, &counter, NUM_CPUS, i);
    }

    // join 8 
    for(auto& t : threads){
        t.join();
    }

    std::cout << get(&counter) << std::endl;
    //expected output is 800024, we get 800000 since after every 100 flush it updates, so last 3 updates per thread is stored in local

    return 0;
}