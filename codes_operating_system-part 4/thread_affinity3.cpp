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
  CPU_ZERO(&cpu_set_0);
  CPU_ZERO(&cpu_set_1);

  // And set the CPU cores we want to pin the threads too
  CPU_SET(0, &cpu_set_0);
  CPU_SET(1, &cpu_set_1);

  // Work function for our threads increast the value of val +1 2^10 times
  auto work = [](AlignedAtomic &atomic){
      for(int i = 0; i < (1 << 10); i++) atomic.val++;
  };
  
  // Create thread 0 and 1, and pin them to core 0
  std::thread t0(work, std::ref(a));
  std::thread t1(work, std::ref(a));

  assert(pthread_setaffinity_np(t0.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_0) == 0);
  assert(pthread_setaffinity_np(t1.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_0) == 0);

  // Create thread 2 and 3, and pin them to core 1
  std::thread t2(work, std::ref(b));
  std::thread t3(work, std::ref(b));
  assert(pthread_setaffinity_np(t2.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_1) == 0);
  assert(pthread_setaffinity_np(t3.native_handle(), sizeof(cpu_set_t),
                                &cpu_set_1) == 0);

  // Join the threads
  t0.join();
  t1.join();
  t2.join();
  t3.join();

  std::cout << "Thread 0 is running on CPU: ";
  for (int i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &cpu_set_0)) {
      std::cout << i << " ";
      cout<<sched_getcpu();
    }
  }
  std::cout << std::endl;

  std::cout << "Thread 1 is running on CPU: ";
  for (int i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &cpu_set_0)) {
      std::cout << i << " ";
      cout<<sched_getcpu();
    }
  }
  std::cout << std::endl;

  std::cout << "Thread 2 is running on CPU: ";
  for (int i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &cpu_set_1)) {
      std::cout << i << " ";
      cout<<sched_getcpu();
    }
  }
  std::cout << std::endl;

  std::cout << "Thread 3 is running on CPU: ";
  for (int i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &cpu_set_1)) {
      std::cout << i << " ";
      cout<<sched_getcpu();
    }
  }
  std::cout << std::endl;
}

int main() {
  thread_affinity();
  return 0;
}
