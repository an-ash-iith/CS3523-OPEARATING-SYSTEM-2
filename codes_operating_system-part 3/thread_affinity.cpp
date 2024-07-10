// Shows how to launch four threads, pinning them to CPUs 0,1,2,3
//
// Note: if your machine has fewer than 4 logical CPUs, you may need to adjust
// the number of threads launched here (or change the affinity in CPU_SET).
//
// Eli Bendersky [http://eli.thegreenplace.net]
// This code is in the public domain.
#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <vector>
// using namespace std;

int main(int argc, const char **argv)
{
    constexpr unsigned num_threads = 4;  //creating 4 number of thread assuming thread must be atleast 1 core associaed with one process
    // A mutex ensures orderly access to std::cout from multiple threads.
    std::mutex iomutex;
    std::vector<std::thread> threads(num_threads); //vector name threads containg data type as thread with size number of threads 
    for (unsigned i = 0; i < num_threads; ++i)
    {
        threads[i] = std::thread([&iomutex, i]
                                 {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      while (1) {
        {
          // Use a lexical scope and lock_guard to safely lock the mutex only
          // for the duration of std::cout usage.
          // std::lock_guard<std::mutex> iolock(iomutex);
          std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << "\n";  
        }

        // Simulate important work done by the thread by sleeping for a bit...
        // this is the place where you can keep your work 
        // like in our case we can keep it here
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
      } });

        // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
        // only CPU i as set.
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i, &cpuset);

        // int rc: This is an integer variable that stores the return code of the pthread_setaffinity_np function. 
        // A return code of 0 indicates success, while a non-zero return code indicates an error.
        int rc = pthread_setaffinity_np(threads[i].native_handle(),sizeof(cpu_set_t), &cpuset);
        if (rc != 0)
        {
            std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
        }
    }

    for (auto &t : threads)
    {
        t.join();
    }
    return 0;
}