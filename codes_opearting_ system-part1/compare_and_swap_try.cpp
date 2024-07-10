#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> lock(false);

void critical_section(int thread_id) {
    bool expected = false;
    bool desired = true;
    while (!lock.compare_exchange_strong(expected, desired)) {
        expected = false;
        desired = true;
    }

    // Critical section
    std::cout << "Thread " << thread_id << " entered the critical section." << std::endl;

    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Exiting the critical section
    std::cout << "Thread " << thread_id << " exited the critical section." << std::endl;
    lock.store(false);
    // std::cout << "Thread " << thread_id << " exited the critical section." << std::endl;
}

int main() {
    std::thread threads[10];
    for (int i = 0; i < 10; ++i) {
        threads[i] = std::thread(critical_section, i);
    }

    for (int i = 0; i < 10; ++i) {
        threads[i].join();
    }

    return 0;
}
