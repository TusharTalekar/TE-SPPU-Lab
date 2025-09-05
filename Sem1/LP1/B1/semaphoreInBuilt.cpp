// Imagine a system that can only handle a certain number of concurrent file downloads, or a database connection pool. A counting semaphore can limit the number of threads performing these operations.

// Since std::counting_semaphore and std::binary_semaphore are C++20 features, we use std::condition_variable in conjunction with std::mutex to emulate their behavior in older C++ versions. std::condition_variable allows threads to wait for a specific condition to become true and to be notified when that condition changes.

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>             // For mutex, unique_lock
#include <condition_variable> // For condition_variable
using namespace std;

// Maximum concurrent downloads
const int MAX_CONCURRENT_DOWNLOADS = 3;
int current_downloads = 0; // Represents the count of active "slots"

mutex mtx_downloads; // Mutex to protect current_downloads
condition_variable cv_downloads; // Condition variable to signal slot availability

void download_file(int file_id) {
    unique_lock<mutex> lock(mtx_downloads); // Acquire lock

    // Wait until a slot is available
    cv_downloads.wait(lock, [] { return current_downloads < MAX_CONCURRENT_DOWNLOADS; });

    current_downloads++; // Acquire a slot
    cout << "Starting download of file " << file_id << ". Active: " << current_downloads << endl;
    lock.unlock(); // Release lock while simulating work, allows other threads to check/acquire

    this_thread::sleep_for(chrono::seconds(2)); // Simulate download time

    lock.lock(); // Re-acquire lock to modify shared state
    current_downloads--; // Release the slot
    cout << "Finished download of file " << file_id << ". Active: " << current_downloads << endl;
    cv_downloads.notify_one(); // Notify one waiting thread that a slot is free
    // lock is automatically released by unique_lock when it goes out of scope
}

int main() {
    int num_files_to_download = 10;
    vector<thread> download_threads;

    cout << "Simulating file downloads with max " << MAX_CONCURRENT_DOWNLOADS << " concurrent downloads ." << endl;

    for (int i = 1; i <= num_files_to_download; ++i) {
        download_threads.emplace_back(download_file, i);
    }

    for (thread& t : download_threads) {
        t.join();
    }

    cout << "All downloads completed." << endl;

    return 0;
}