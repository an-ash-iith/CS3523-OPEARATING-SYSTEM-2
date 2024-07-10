#include <iostream>
#include <atomic>
#include <pthread.h>
#include <unistd.h>

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
    std::atomic<bool> lock_ = false;
};

struct ThreadData {
    SpinLock* spin_lock;
    int K;
};

std::atomic<int> counter = 0;

void* threadFunction(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    SpinLock* spin_lock = data->spin_lock;
    int K = data->K;
    for (int i = 0; i < K; ++i) {
        spin_lock->lock();
        // Increment the shared counter
        ++counter;
        spin_lock->unlock();
    }
    return nullptr;
}

int main() {
    SpinLock spin_lock;
    const int K = 1000;

    pthread_t threads[16];
    ThreadData thread_data[16];
    for (int i = 0; i < 16; ++i) {
        thread_data[i].spin_lock = &spin_lock;
        thread_data[i].K = K;
        pthread_create(&threads[i], nullptr, threadFunction, &thread_data[i]);
    }

    for (int i = 0; i < 16; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "Counter value: " << counter << std::endl;

    return 0;
}
