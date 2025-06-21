#pragma once
#include <vector>
#include "../utils/message.h"

class QueueNode{

    public:
        // Initializer allocates N=capacity Messages
        void init(size_t capacity);

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
    
    private:
        // Preallocated N messages (fixed size)
        std::vector<Message> slab;

        // Holds pointers to existing Messages in slab. This will get rid 
        // of allocation overhead. enqueue/dequeue operations will never 
        // create/delete new Message objects but just modify the underlying
        std::vector<Message*> queue;

        int front_ptr;
        int back_ptr;
        size_t max_size;
};