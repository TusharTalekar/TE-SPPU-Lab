#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

#define BUFFER_SIZE 5

class Semaphore {
private:
    volatile int count;

public:
    Semaphore(int initial_count) : count(initial_count) {}

    void wait() {
        while (true) {
            while (count <= 0) {}  // busy wait
            if (__sync_bool_compare_and_swap(&count, count, count - 1)) {
                break;
            }
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

    Semaphore emptySlots;  // count of empty slots
    Semaphore fullSlots;   // count of full slots

public:
    ProducerConsumer() : emptySlots(BUFFER_SIZE), fullSlots(0) {}

    void produce(int item) {
        emptySlots.wait();   // wait if no empty slots

        // CRITICAL SECTION START - but no mutex here!
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        cout << "Produced: " << item << endl;
        // CRITICAL SECTION END

        fullSlots.signal();  // signal one more full slot
    }

    int consume() {
        fullSlots.wait();    // wait if no full slots

        // CRITICAL SECTION START - no mutex protection
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        cout << "Consumed: " << item << endl;
        // CRITICAL SECTION END

        emptySlots.signal(); // signal one more empty slot

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
        pc.consume();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    ProducerConsumer pc;
    int itemCount = 10;

    cout << "=== Producer-Consumer with semaphore ===" << endl;

    thread producer(producerThread, ref(pc), itemCount);
    thread consumer(consumerThread, ref(pc), itemCount);

    producer.join();
    consumer.join();

    cout << "=== DONE ===" << endl;
    return 0;
}
