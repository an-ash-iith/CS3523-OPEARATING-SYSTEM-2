#include <iostream>
#include <thread>
#include <vector>
#include <sched.h>
#include <unistd.h>

void threadFunc(int threadId) {
    // Get the CPU core on which this thread is running
    int cpu = sched_getcpu();
    usleep(100);
    std::cout << "Thread " << threadId << " is running on CPU " << cpu << std::endl;

    // Do some work here
    usleep(1000); // Sleep for 1 second
}

int main() {
    // Create 4 threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.push_back(std::thread([i]() {
            // Set the CPU affinity for this thread
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset); // Set this thread to run on core i
            sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
            threadFunc(i);
        }));
    }

    // Join the threads
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
