#include <pthread.h>
#include <atomic>
#include <cassert>
#include <thread>
#include <iostream>
using namespace std;

// Aligned type to avoid atomics on the same cache line
struct AlignedAtomic {
  alignas(64) std::atomic<int> val = 0;
};

void thread_affinity() {
  AlignedAtomic a;
  AlignedAtomic b;

  // Create cpu sets for threads 0,1 and 2,3
  cpu_set_t cpu_set_0;
  cpu_set_t cpu_set_1;

  // Zero them out
//   The CPU_ZERO macro is used to initialize a CPU set to the empty set.
//  A CPU set is a data structure that represents a set of CPU cores. 
// The CPU_ZERO macro clears all the bits in the CPU set, effectively creating an empty set.
  CPU_ZERO(&cpu_set_0);
  CPU_ZERO(&cpu_set_1);

  // And set the CPU cores we want to pin the threads too
  CPU_SET(0, &cpu_set_0);
  CPU_SET(1, &cpu_set_1);

  // Work function for our threads increast the value of val  2^11 times
  auto work = [](AlignedAtomic &atomic){
      for(int i = 0; i < (1 << 10); i++) atomic.val++;
  };
  
  // Create thread 0 and 1, and pin them to core 0
  // this is c++ way of creating the threads----pthread create can be used but with few changes
  std::thread t0(work, std::ref(a));
  std::thread t1(work, std::ref(a));
cout<<"Thread 0 and 1 running on "<<sched_getcpu()<<endl;

//   pthread_create(&t0,NULL,work,&a);

  assert(pthread_setaffinity_np(t0.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_0) == 0);
  assert(pthread_setaffinity_np(t1.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_0) == 0);
  // Create thread 2 and 3, and pin them to core 1
  std::thread t2(work, std::ref(b));
  std::thread t3(work, std::ref(b));
  cout<<"Thread 1 and 2 running on "<<sched_getcpu()<<endl;
  assert(pthread_setaffinity_np(t2.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_1) == 0);
  assert(pthread_setaffinity_np(t3.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_1) == 0);

// 
// if you will print here you might get wrong value this not because of the race condition but you are printing even before
// the thread has completed his work ----atomic variable ensures that there is no race condititon 

// std::cout<<"Value return by thread 0 and 1 is "<<a.val<<std::endl;
// std::cout<<"Value return by thread 1 and 2 is "<<b.val<<std::endl;

  // Join the threads
    std::cout << "Thread 0 is running on CPU: " << sched_getcpu() << std::endl;
  t0.join();
    std::cout << "Thread 1 is running on CPU: " << sched_getcpu() << std::endl;
  t1.join();
    std::cout << "Thread 2 is running on CPU: " << sched_getcpu() << std::endl;
  t2.join();
    std::cout << "Thread 3 is running on CPU: " << sched_getcpu() << std::endl;
  t3.join();

// calling the things after the thread is terminated

std::cout<<"Value return by thread 0 and 1 is "<<a.val<<std::endl;
std::cout<<"Value return by thread 1 and 2 is "<<b.val<<std::endl;
}

int main() {
  thread_affinity();
  return 0;
}
