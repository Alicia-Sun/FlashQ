#pragma once
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include "../utils/message.h"
#include "../utils/helpers.h"

class QueueNode{

    public:
        int id;
        bool is_primary;
        uint64_t max_payload_size;
        std::unordered_map<int, ServerConfig> server_configs;

        // Initializer allocates N=capacity Messages
        int init(int node_id, size_t capacity, uint64_t max_msg_size, std::string config_file);

        // Modifies underlying Message at back_ptr with new data and length
        int enqueue(int msg_id, const char* data, size_t data_len);

        // Returns copy of Message and moves front_ptr at the end, ensuring
        // no concurrent enqueues overwrite it mid operation. RVO copies it
        // directly into the callers stack frame
        Message dequeue();

        // WARNING: USE WITH CAUTION
        // Returns the Message id (by value) and pointer to Message. This 
        // alternative avoids the overhead of copying, but is dangerous.
        // It is the responsibility of the caller to ensure that the returned 
        // id matches the id of the pointer's Message. If it doesn't, then that
        // pointer is now referring to new data.
        uint32_t dequeue_by_ptr(Message*& out_msg_ptr);
        // ^^ side note, perhaps in consumer when you dequeue_by_ptr, first check if id matches
        // if so then send(m.data()...), and after sending check one more time if the id still matches
        // if not, then the message contents might not be accurate so send(WARNING) to the client

        // Client handler
        void handle_client(int clientfd);
    
    private:
        int listener_socketfd;

        std::vector<int> socket_vec;
        std::vector<std::thread> thread_vec;

        // Preallocated N messages (fixed size)
        std::vector<Message> slab;

        // Holds pointers to existing Messages in slab. This will get rid 
        // of allocation overhead. enqueue/dequeue operations will never 
        // create/delete new Message objects but just modify the underlying
        std::vector<Message*> queue;

        // Points to the index of the first element; -1 if none yet
        std::atomic<int> head;

        // Points to the index of the last element; -1 if none yet
        std::atomic<int> tail;

        size_t capacity;

        // Each sequence[i] encodes the state of slot i
        // 4 states based on sequence[i] equals...
        // tail: writable and checked by producer
        // head + 1: readable and checked by consumer
        // < tail: claimed (stale) queue is full and producer backs off
        // > tail: claimed (future) racing producer and this producer backs off
        std::unique_ptr<std::atomic<size_t>[]> sequence;
};