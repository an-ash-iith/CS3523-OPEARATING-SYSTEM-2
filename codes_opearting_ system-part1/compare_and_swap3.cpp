#include <iostream>
#include <atomic>
#include <pthread.h>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;
class SpinLock {
public:
    void lock() {
        bool expected = false;
        while (!lock_.compare_exchange_strong(expected, true, std::memory_order_acquire)) {
            // Busy wait until the lock becomes available
            expected = false;
        }
    }

    void unlock() {
        lock_.store(false, std::memory_order_release);
    }

private:
    // std::atomic<bool> lock_ = false;
     std::atomic<bool> lock_ = false;
};

typedef struct
{
    // vector<int> ro vector<vector<int>> *rows;w;
    int *count1;
    SpinLock* spin_lock;
} arguments;



int counter = 0;

void* threadFunction(void* arg) {

    arguments* data = static_cast<arguments*>(arg);
    SpinLock* spin_lock = data->spin_lock;

    for (int i = 0; i < 10000; ++i) {
        spin_lock->lock();
        // Increment the shared counter
        ++counter;
        spin_lock->unlock();
    }
    return nullptr;
}

int main() {
  
    SpinLock spin_lock;
    int count = 0;
    int K =16;
    arguments arg1[K];


    pthread_t threads[16];
    for (int i = 0; i < 16; ++i) {
        arg1[i].count1 = &count;
        arg1[i].spin_lock = &spin_lock;
        pthread_create(&threads[i], nullptr, threadFunction, &arg1[i]);
    }

    for (int i = 0; i < 16; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "Counter value: " << counter << std::endl;

    return 0;
}
