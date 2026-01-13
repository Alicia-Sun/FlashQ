#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <vector>

#include "message.h"
#include "../utils/helpers.h"

class FlashQ {

    public:
        ServerConfig server_config;

        // Maximum number of bytes a Message in the queue can hold.
        uint64_t max_payload_size;

        // Initializer allocates N=capacity Messages.
        int init(int capacity, uint64_t max_msg_size, std::string config_file);

        // Modifies underlying Message corresponding to tail_'s queue spot with 
        // data. If queue is not ready (ex. it is full or racing producer), 
        // spins.
        int enqueue(int msg_id, const char* data, size_t data_len);

        // Copies underlying Message corresponding to head_'s queue spot and 
        // and returns it. If queue is not ready (ex. it is full or racing 
        // consumer), spins. 
        Message dequeue();

        // Client handler.
        void handle_client(int clientfd);
    
    private:
        int listener_socketfd_;
        std::vector<int> socket_vec_;
        std::vector<std::thread> thread_vec_;

        // ******** QUEUE STATE ******** //

        // Points to the index of the first element. Essentially an ever 
        // increasing global position of queue head which maps to queue slot 
        // using mask. AKA pos.
        std::atomic<int> head_;

        // Points to the index of the last element. Essentially an ever 
        // increasing global position of queue tail which maps to queue slot 
        // using mask. AKA pos.
        std::atomic<int> tail_;

        // Used to derive queue slot (= pos & mask).
        int mask_;

        // Maximum number of messages queue can hold.
        int capacity_;

        // Actual queue holding preallocated N messages in a fixed size slab 
        // where queue[i] holds the data for slot i.
        std::vector<Message> queue_;

        // Each sequence[i] encodes the state of slot i. There are 4 states 
        // based on what sequence[i] equals:
        //      1. tail: empty slot and writable only
        //      2. head + 1: full and readable only
        //      3. < tail: claimed (stale) queue is full and producer backs off
        //      4. > tail: claimed (future) racing producer and this producer 
        //      backs off
        std::unique_ptr<std::atomic<size_t>[]> sequence_;
};