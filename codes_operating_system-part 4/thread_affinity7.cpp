#include <iostream>
#include <vector>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
using namespace std;

#define number_of_core as nc
#define nc 8

pthread_mutex_t mutex;
void *threadFunc(void *arg)
{    int cpu = sched_getcpu();
    int threadId = *(int *)arg;
    // Get the CPU core on which this thread is running
   

    pthread_mutex_lock(&mutex);
    std::cout << "Thread " << threadId << " is running on CPU " << cpu << std::endl;
    pthread_mutex_unlock(&mutex);
    // Do some work here
    usleep(1000); // Sleep for 1 second

    return NULL;
}

int main()
{
    // Create 4 threads
    pthread_mutex_init(&mutex, NULL);
    std::vector<pthread_t> threads;

    for (int i = 0; i < 40; ++i)
    {
        pthread_t thread;
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        int *threadId = new int(i);
        CPU_SET(*threadId % nc, &cpuset);
        pthread_attr_t attr;
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

        pthread_create(&thread, &attr, threadFunc, (void *)threadId);
        threads.push_back(thread);

        // Set the CPU affinity for this thread

        //  cout<<"Number of cpu in the set is "<<CPU_COUNT(&cpuset)<<endl;
        //  int cpus = CPU_COUNT(&cpuset);

        // cout << "Number of cpu in the set is " << CPU_COUNT(&cpuset) << endl;
        // pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
        // usleep(100);
    }

    // Join the threads
    for (auto &thread : threads)
    {
        pthread_join(thread, NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
