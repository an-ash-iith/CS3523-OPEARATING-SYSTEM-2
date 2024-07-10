#include <iostream>
#include <atomic>
#include <thread>
#include <pthread.h>
using namespace std;

class SpinLock
{
public:
    void lock()
    {
        bool expected = false;
        while (!lock_.compare_exchange_strong(expected, true, std::memory_order_acquire))
        {
            // Busy wait until the lock becomes available
            expected = false;
        }
    }

    void unlock()
    {
        lock_.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> lock_ = false;
};

void *calculate(void *count)
{
    SpinLock spin_lock;

    spin_lock.lock();
    int *countPtr = (int *)count; // Cast the void pointer to int pointer

    for (int i = 0; i < 1000; i++)
    {
        (*countPtr)++; // Increment the value pointed to by countPtr
    }
    spin_lock.unlock();
    return NULL;
}

int main()
{

    int K = 16;
    pthread_t thread[K];

    //   K -- number of thread

    int count = 0;

    for (int i = 0; i < K; i++)
    {
        pthread_create(thread + i, NULL, &calculate, (void *)&count);
    }

    for (int i = 0; i < K; i++)
    {
        pthread_join(thread[i], NULL);
    }

    cout << count << endl;

    return 0;
}
