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
            // spinning
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }

    void unlock(string s,int i) {
        cout<<s<<" "<<i<<" unlocked"<<endl;
        __sync_lock_release(&locked);
    }
};

class ProducerConsumer {
private:
    int buffer[BUFFER_SIZE];
    int in = 0, out = 0;
    int count = 0;  

    Mutex mutex;

public:
    void produce(int item) {
        // Wait until there is space in buffer (busy wait)
        while (true) {
            mutex.lock("producer",item);
        cout<<"producer "<<item<<" locked"<<endl;
            if (count < BUFFER_SIZE) {
                // There is space, produce item
                buffer[in] = item;
                in = (in + 1) % BUFFER_SIZE;
                count++;
                this_thread::sleep_for(chrono::milliseconds(1000));
                cout << "Produced: " << item << endl;
                mutex.unlock("producer",item);
                break;  // exit busy wait loop
            }
            mutex.unlock("producer",item);
            cout<<"Producer "<<item<<" busy waiting"<<endl;
            // Sleep a bit to reduce CPU burning 
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    int consume(int i) {
        int item = -1;

        // Wait until there is at least one item to consume (busy wait)
        while (true) {
            mutex.lock("consumer",i);
        cout<<"consumer "<<i<<" locked"<<endl;
            if (count > 0) {
                // There is an item, consume it
                item = buffer[out];
                out = (out + 1) % BUFFER_SIZE;
                count--;
                this_thread::sleep_for(chrono::milliseconds(100));
                cout << "Consumed: " << item << endl;
                mutex.unlock("consumer",i);
                break;  // exit busy wait loop
            }
            mutex.unlock("consumer",i);
            cout<<"Consumer "<<i<<" busy waiting"<<endl;
            // Sleep a bit to reduce CPU burning while spinning
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        return item;
    }
};

// Producer thread function
void producerThread(ProducerConsumer &pc, int count) {
    for (int i = 1; i <= count; ++i) {
        pc.produce(i);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Consumer thread function
void consumerThread(ProducerConsumer &pc, int count) {
    for (int i = 1; i <= count; ++i) {
        pc.consume(i);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    ProducerConsumer pc;
    int itemCount = 4;

    cout << "=== Producer-Consumer with custom Mutex ===" << endl;

    thread producer(producerThread, ref(pc), itemCount);
    thread consumer(consumerThread, ref(pc), itemCount);

    producer.join();
    consumer.join();

    cout << "=== DONE ===" << endl;
    return 0;
}
