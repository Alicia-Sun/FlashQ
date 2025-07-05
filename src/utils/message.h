#pragma once
#include <array>
#include <vector>
#include "stdint.h"

class Message {
    public:
        uint64_t max_payload_size;
        Message(uint64_t new_msg_max_size) {
            max_payload_size = new_msg_max_size;
            payload.resize(max_payload_size);
        }
        // 64 KB
        // static const uint64_t max_payload_size { uint64_t(64) * 1024 };

        // Getters
        uint32_t get_id() const { return msg_id; }
        uint64_t get_length() const { return max_payload_size; }
        const char* get_payload() { return payload.data(); }

        int write_new_paylaod(uint32_t new_msg_id, const char* new_payload, uint64_t new_length);

    private:
        uint32_t msg_id {0};
        uint64_t length {0};

        std::vector<char> payload {};
};