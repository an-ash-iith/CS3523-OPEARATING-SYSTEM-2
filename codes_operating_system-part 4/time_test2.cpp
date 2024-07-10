#include <iostream>
#include <chrono>
#include <string>

using namespace std;

string getCurrentTimeStamp() {
    // Get the current time point
    auto now = chrono::system_clock::now();
    
    // Convert to microseconds since epoch
    auto us = chrono::time_point_cast<chrono::microseconds>(now);
    auto epoch = us.time_since_epoch();
    auto value = chrono::duration_cast<chrono::microseconds>(epoch);
    chrono::microseconds current_us = chrono::duration_cast<chrono::microseconds>(value);

    // Calculate hours, minutes, seconds, milliseconds, and microseconds
    auto hours = chrono::duration_cast<chrono::hours>(current_us);
    current_us -= hours;
    auto minutes = chrono::duration_cast<chrono::minutes>(current_us);
    current_us -= minutes;
    auto seconds = chrono::duration_cast<chrono::seconds>(current_us);
    current_us -= seconds;
    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(current_us);
    current_us -= milliseconds;
    
    // Format the time stamp
    return
           to_string(milliseconds.count()) + ":" +
           to_string(current_us.count());
}


int main() {
    // Call the function to get the current time stamp
    std::string timeStamp = getCurrentTimeStamp();

    // Print the time stamp
    std::cout << "Current Time Stamp: " << timeStamp << std::endl;
    
    timeStamp = getCurrentTimeStamp();
    // Print the time stamp
    std::cout << "Current Time Stamp: " << timeStamp << std::endl;

    timeStamp = getCurrentTimeStamp();
        // Print the time stamp
    std::cout << "Current Time Stamp: " << timeStamp << std::endl;

    timeStamp = getCurrentTimeStamp();
        // Print the time stamp
        // Print the time stamp
    std::cout << "Current Time Stamp: " << timeStamp << std::endl;
    return 0;
}
