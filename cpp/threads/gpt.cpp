#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

// Define a message queue class that will hold messages to be sent between threads
class MessageQueue {
public:
    // Add a message to the queue
    void push(const string& message) {
        lock_guard<mutex> lock(mutex_);
        queue_.push(message);
        cond_var_.notify_one();
    }

    // Get the next message from the queue (blocks until a message is available)
    string pop() {
        unique_lock<mutex> lock(mutex_);
        cond_var_.wait(lock, [this] { return !queue_.empty(); });
        string message = queue_.front();
        queue_.pop();
        return message;
    }

private:
    queue<string> queue_;
    mutex mutex_;
    condition_variable cond_var_;
};

// Define a function that will run in each thread and send messages to the message queue
void sendMessages(MessageQueue& queue, int threadId) {
    for (int i = 0; i < 5; ++i) {
        // Generate a message with the thread ID and message number
        string message = "Thread " + to_string(threadId) + " message " + to_string(i);

        // Send the message to the message queue
        queue.push(message);
    }
}

// Define a function that will run in the receiver thread and read messages from the message queue
void receiveMessages(MessageQueue& queue) {
    while (true) {
        // Get the next message from the message queue (blocks until a message is available)
        string message = queue.pop();

        // Print the message to the console
        cout << "Received message: " << message << endl;

        // Check if we've received all the messages
        if (message == "Thread 3 message 4") {
            break;
        }
    }
}

int main() {
    // Create a message queue object
    MessageQueue queue;

    // Create three sender threads and one receiver thread
    thread t1(sendMessages, ref(queue), 1);
    thread t2(sendMessages, ref(queue), 2);
    thread t3(sendMessages, ref(queue), 3);
    thread t4(receiveMessages, ref(queue));

    // Wait for all threads to finish
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
