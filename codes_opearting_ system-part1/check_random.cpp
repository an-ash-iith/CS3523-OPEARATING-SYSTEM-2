#include <iostream>
#include <random>

using namespace std;
// Function to generate a random delay time for CS (Critical Section)
double randCSTime(double muCS) {
    // Create a random number generator engine
    random_device rand;
    mt19937 gen(rand());
    
    // Create an exponential distribution with average muCS
    exponential_distribution<double> dist(1.0 / muCS);

    // Generate a random delay time for CS
    return dist(gen);
}

// Function to generate a random delay time for Rem (Remaining Time)
double randRemTime(double muRem) {
    // Create a random number generator engine
    random_device rand;
    mt19937 gen(rand());
    
    // Create an exponential distribution with average muRem
    exponential_distribution<double> dist(1.0 / muRem);

    // Generate a random delay time for Rem
    return dist(gen);
}

int main() {
    // Set the average delay times for CS and Rem
    double muCS = 10.0; // Average CS delay in milliseconds
    double muRem = 5.0; // Average Rem delay in milliseconds
    
    // Generate and print random delay times for CS and Rem
    cout << "Random delay time for CS: " << randCSTime(muCS) << " ms\n";
    cout << "Random delay time for Rem: " << randRemTime(muRem) << " ms\n";
    
    return 0;
}
