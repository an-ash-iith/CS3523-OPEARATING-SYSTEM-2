#include <iostream>
#include <chrono>

long long getCurrentTimeMicroseconds() {
    // Get the current time point
    auto now = std::chrono::high_resolution_clock::now();

    // Convert the time point to microseconds since the epoch
    return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}

int main() {
    // Call the function to get the current time in microseconds
    long long microseconds_since_epoch = getCurrentTimeMicroseconds();

    // Output the time
    std::cout << "Current time in microseconds since epoch: " << microseconds_since_epoch << std::endl;

    return 0;
}
