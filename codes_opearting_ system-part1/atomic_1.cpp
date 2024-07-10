#include <iostream>
#include <atomic>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    std::atomic<int>* count1;
    int N;
} arguments;

void* threadFunction(void* arg1) {
    arguments *threadArgs = (arguments *)arg1;
    std::atomic<int>* counter = threadArgs->count1;

    for (int i = 0; i < 1000; ++i) {
        // Increment the shared counter
        ++(*counter);
    }
    return nullptr;
}

int main() {
    std::atomic<int> counter = 0;

    int K = 16;
    arguments arg1[K];
    pthread_t threads[K];

    for (int i = 0; i < 15; ++i) {
        arg1[i].count1 = &counter;
        pthread_create(&threads[i], nullptr, threadFunction, (void *)&arg1[i]);
    }

    for (int i = 0; i < 16; ++i) {
        pthread_join(threads[i], nullptr);
    } 

    std::cout << "Counter value: " << counter << std::endl;

    return 0;
}
