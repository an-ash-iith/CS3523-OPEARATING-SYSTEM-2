#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

atomic<int> turn(0);
atomic<bool> flag[2] = {false, false};

void process_0() {
    flag[0] = true;
    turn = 1;
    while (flag[1] && turn == 1) { /* Wait */ }
    // Critical section
    cout << "Process 0 is in critical section" << endl;
    // End of critical section
    flag[0] = false;
}

void process_1() {
    flag[1] = true;
    turn = 0;
    while (flag[0] && turn == 0) { /* Wait */ }
    // Critical section
    cout << "Process 1 is in critical section" << endl;
    // End of critical section
    flag[1] = false;
}

int main() {
    thread t1(process_0);
    thread t2(process_1);
    
    t1.join();
    t2.join();

    return 0;
}
