#pragma once

#include <stdint.h>

#include <array>
#include <vector>


enum class PayloadSize : uint64_t {
    SMALL  = 1024,     // 1 KB
    MEDIUM = 4096,     // 4 KB
    LARGE  = 16384,    // 16 KB
    XLARGE = 65536     // 64 KB
};

class Message {
    public:
        uint64_t max_payload_size;
        Message(uint64_t new_msg_max_size) {
            max_payload_size = new_msg_max_size;
            payload_.resize(max_payload_size);
        }
        
        Message(const Message& other_msg);
        Message(Message&& other) noexcept;

        // ******** Getters ******** //
        uint32_t get_id() const { return msg_id_; }
        uint64_t get_length() const { return max_payload_size; }
        const char* get_payload() { return payload_.data(); }

        // Replaces the existing payload content for this message with 
        // new_payload.
        int write_new_msg(uint32_t new_msg_id, const char* new_payload, 
            uint64_t new_length
        );

        // Returns a copy of this Message to the caller.
        Message pop_msg();

    private:
        uint32_t msg_id_ {0};
        uint64_t length_ {0};
        std::vector<char> payload_ {};
};

PayloadSize parse_payload_size(int size_code);