// Imagine multiple threads incrementing a shared counter. Without a mutex, the increments could be lost due to race conditions (e.g., two threads read the same value, increment it, and then both write back the "same" new value).

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

long long shared_counter = 0;
mutex counter_mutex;

// A more idiomatic C++ way to use std::mutex is with std::lock_guard for RAII (Resource Acquisition Is Initialization), which ensures the mutex is automatically unlocked when the lock_guard goes out of scope, even if an exception occurs.

void increment_counter(int n) {
    for (int i = 0; i < n; ++i) {
        // lock_guard<mutex> lock(counter_mutex);
        
        // Acquire the lock before accessing shared_counter
        counter_mutex.lock();
        shared_counter++;
        // Release the lock after accessing shared_counter
        counter_mutex.unlock();
    }
}


int main() {
    int num_threads = 4;
    int increments_per_thread = 100000;
    vector<thread> threads;

    cout << "Starting threads to increment a shared counter..." << endl;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment_counter, increments_per_thread);
    }

    for (thread& t : threads) {
        t.join();
    }

    cout << "Final counter value: " << shared_counter << endl;
    // Expected value: num_threads * increments_per_thread
    cout << "Expected counter value: " << (long long)num_threads * increments_per_thread << endl;

    return 0;
}