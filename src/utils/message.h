#pragma once
#include <array>
#include "stdint.h"

class Message {
    public:
        static const uint64_t max_payload_size { uint64_t(256) * 256 * 256 * 256 };

        uint32_t get_id() const { return msg_id; }
        uint64_t get_length() const { return max_payload_size; }
        const char* get_payload() { return payload.data(); }

        int write_new_paylaod(uint32_t new_msg_id, const char* new_payload, uint64_t new_length);

    private:
        uint64_t length {0};
        uint32_t msg_id {0};
        
        std::array<char, max_payload_size> payload {};
};