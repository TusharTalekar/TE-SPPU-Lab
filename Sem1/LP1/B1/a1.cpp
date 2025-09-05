#include <iostream>      
#include <thread>        
#include <chrono>   
using namespace std;

#define BUFFER_SIZE 5

class Mutex {
private:
    volatile bool locked;

public:
    Mutex() : locked(false) {}

    void lock(string s,int i) {
        if(locked)  cout<<s<<" "<<i<<" waiting"<<endl; 
        while (__sync_lock_test_and_set(&locked, true)) {
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }

    void unlock(string s,int i) {
        cout<<s<<" "<<i<<" unlocked"<<endl;
        __sync_lock_release(&locked);
    }
};

class Semaphore {
private:
    volatile int count;

public:
    Semaphore(int initial_count) : count(initial_count) {}

    void wait(string s,int i) {
        while (true) {
            while (count <= 0) {
                // busy wait
            //     cout<<s<<" "<<i<<" busy waiting"<<endl;
            // this_thread::sleep_for(chrono::milliseconds(1000));
            } 
            if (__sync_bool_compare_and_swap(&count, count, count - 1)) {
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    void signal() {
        __sync_fetch_and_add(&count, 1);
    }
};


class ProducerConsumer {
private:
    int buffer[BUFFER_SIZE];
    int in = 0, out = 0;

    Mutex mutex;
    Semaphore emptySlots;
    Semaphore fullSlots;

public:
    ProducerConsumer() : emptySlots(BUFFER_SIZE), fullSlots(0) {}

    void produce(int i) {
        emptySlots.wait("producer",i);  
        mutex.lock("producer",i);
        cout<<"producer "<<i<<" locked"<<endl;

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        this_thread::sleep_for(chrono::milliseconds(1000));
        cout << "Produced: " << i << endl;

        mutex.unlock("producer",i);
        fullSlots.signal();
    }

    int consume(int i) {
        fullSlots.wait("consumer",i);
        mutex.lock("consumer",i);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "Consumed: " << item << endl;

        mutex.unlock("consumer",i);
        emptySlots.signal();

        return item;
    }
};


void producerThread(ProducerConsumer &pc, int count) {
    for (int i = 1; i <= count; ++i) {
        pc.produce(i);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
void consumerThread(ProducerConsumer &pc, int count) {
    for (int i = 1; i <= count; ++i) {
        pc.consume(i);
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

int main() {
    ProducerConsumer pc;
    int itemCount = 4;

    cout << "--- Multithreaded Producer-Consumer ---" << endl;

    thread producer(producerThread, ref(pc), itemCount);
    thread consumer(consumerThread, ref(pc), itemCount);

    // Wait for threads to finish


    producer.join();
    consumer.join();

    cout<<"Hello World"<<endl;

    return 0;
}
