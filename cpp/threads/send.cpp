// Using a queue to send messages between threads
//
// Code adapted from https://stackoverflow.com/questions/16049814/passing-messages-between-threads-and-memory-management

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono_literals;

// Structs are the same as classes but everything is public by default
struct MyMessageType
{
    int from;   // who send it
    int val;    // the value sent
    MyMessageType(int a, int b) : from(a), val(b) {}
};

// used to ensure two threads don't try to access same memory at same time
// mutex = mutally exclusive
std::mutex my_mutex;

// random number generator
std::mt19937_64 eng{std::random_device{}()}; // or seed however you want
std::uniform_int_distribution<> dist{100, 1000};

// A queue used to send messages
queue<MyMessageType> my_queue;

void do_work() {
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
}

// Threads used to send messages
void sender(int id)
{
    for (int i = 0; i < 5; ++i)
    {
        // wait random time
        do_work();
        {
            // lock the queue
            const std::lock_guard<std::mutex> lock(my_mutex);
            // make a message
            MyMessageType msg(id, i);
            cout << "Sending " << id << ", " << i << "\n";
            // and send
            my_queue.push(msg);
        } // lock released when it goes out of scope
    }
}

// Thread reciving the messages
void receiver()
{
    while (true)
    {
        // more premative way of locking threads
        my_mutex.lock();
        // loop until there is something on the queue
        while (my_queue.empty())
        {
            my_mutex.unlock();
            // possibly yield to another thread
            my_mutex.lock();
        }
        // Get the message to process
        auto msg = my_queue.front();
        my_queue.pop();
        my_mutex.unlock();
        cout << "Message received from " << msg.from << " val " << msg.val << "\n";
        // wait random time
        do_work();

        // Need a way of terminating  
        if(msg.from == 0) break;
    }
}

// Driver code
int main()
{
    // This thread is launched by using
    // function pointer as callable

    thread th0(receiver);
    thread th11(sender, 1);
    thread th12(sender, 2);
    thread th13(sender, 3);

    // wait for all sending threads to end
    th11.join();
    th12.join();
    th13.join();

    // Message to signal end of receiving loop
    MyMessageType msg(0, 0);
    my_mutex.lock();
    my_queue.push(msg);
    my_mutex.unlock();

    th0.join();
}