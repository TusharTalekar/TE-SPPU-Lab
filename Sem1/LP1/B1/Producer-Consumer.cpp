#include <iostream>        
#include <thread>          
#include <vector>          
#include <mutex>         
#include <condition_variable> 
#include <chrono>          
#include <queue>
#include <atomic> 
using namespace std;

const int BUFFER_SIZE = 5;       
const int ITEMS_TO_PRODUCE = 3; 
const int ITEMS_TO_CONSUME = 3; 
const int NUM_PRODUCERS = 2;     
const int NUM_CONSUMERS = 2;     

queue<int> buffer;             
mutex mtx;                     
condition_variable cv_not_full; 
condition_variable cv_not_empty; 

std::atomic_bool mutex_locked(false);

void log_mutex_state(int id, const string& type, const string& action) {
    if (action == "attempt_lock") {
        if (mutex_locked.load(memory_order_acquire)) {
            cout << type << " " << id << ": Mutex is currently LOCKED. Waiting to acquire it..." << endl;
        } else {
            cout << type << " " << id << ": Attempting to Lock mutex." << endl;
        }
    } else if (action == "locked") {
        mutex_locked.store(true, memory_order_release);
        cout << type << " " << id << ": Mutex Locked." << endl;
    } else if (action == "unlocked") {
        mutex_locked.store(false, memory_order_release);
        cout << type << " " << id << ": Mutex Unlocked." << endl;
    }
}

void producer(int id) {
    for (int i = 0; i < ITEMS_TO_PRODUCE; ++i) {
        int item = (id * ITEMS_TO_PRODUCE) + i + 1; // Create a unique item value

        log_mutex_state(id, "Producer", "attempt_lock");

        unique_lock<mutex> lock(mtx);

        log_mutex_state(id, "Producer", "locked");
        
        cv_not_full.wait(lock, [] {
            return buffer.size() < BUFFER_SIZE; 
        });
        
        // Critical Section
        buffer.push(item); 
        cout << "Producer " << id << ": Produced " << item << " (Buffer size: " << buffer.size() << ")" << endl;
        // end critical Section
        
        cv_not_empty.notify_one();
        log_mutex_state(id, "Producer", "unlocked");
        lock.unlock();

        this_thread::sleep_for(chrono::milliseconds(1000));
    }
     cout << "Producer " << id << ": Finished producing all items." << endl;
}

void consumer(int id) {
    for (int i = 0; i < ITEMS_TO_CONSUME; ++i) {
        log_mutex_state(id, "Consumer", "attempt_lock");
        unique_lock<mutex> lock(mtx);
        log_mutex_state(id, "Consumer", "locked");

        cv_not_empty.wait(lock, [] {
            return !buffer.empty(); 
        });

        // Critical Section
        int item = buffer.front(); 
        buffer.pop();              
        cout << "Consumer " << id << ": Consumed " << item << " (Buffer size: " << buffer.size() << ")" << endl;
        // End Critical Section

        cv_not_full.notify_one();
        log_mutex_state(id, "Consumer", "unlocked");
        lock.unlock();


        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}


int main(){
    cout << "--- Producer-Consumer Simulation (C++11/14/17) ---" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << endl;
    cout << "Items per Producer: " << ITEMS_TO_PRODUCE << endl;
    cout << "Items per Consumer: " << ITEMS_TO_CONSUME << endl;
    cout << "Number of Producers: " << NUM_PRODUCERS << endl;
    cout << "Number of Consumers: " << NUM_CONSUMERS << endl;
    cout << "---------------------------------------------------" << endl;

    vector<thread> producer_threads;
    vector<thread> consumer_threads;


    for (int id = 0; id < NUM_PRODUCERS; ++id) {
        producer_threads.emplace_back(producer, id + 1); 
    }
    for (int id = 0; id < NUM_CONSUMERS; ++id) {
        consumer_threads.emplace_back(consumer, id + 1); // Pass consumer ID
    }

    cout << "\nWaiting for producer threads to finish..." << endl;
    for (thread& t : producer_threads) {
        t.join(); // Blocks main thread until 't' completes
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "All producer threads finished." << endl;

    cout << "Waiting for consumer threads to finish..." << endl;
    for (thread& t : consumer_threads) {
        t.join(); // Blocks main thread until 't' completes
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "All consumer threads finished." << endl;

    cout << "\n--- Simulation Complete ---" << endl;

    return 0;
}