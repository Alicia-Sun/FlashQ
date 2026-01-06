#include "message.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

// new_length must include null terminator
int Message::write_new_paylaod(uint32_t new_msg_id, const char* new_payload, uint64_t new_length) {
    if (new_length > max_payload_size) {
        return -1;
    }

    msg_id = new_msg_id;
    length = new_length;

    memcpy(payload.data(), new_payload, new_length);
    return 0;
}

PayloadSize parse_payload_size(int size_code) {
    switch (size_code) {
        case 0: return PayloadSize::SMALL;
        case 1: return PayloadSize::MEDIUM;
        case 2: return PayloadSize::LARGE;
        case 3: return PayloadSize::XLARGE;
        default:
            perror("Invalid payload size code. Must be 0–3.\n");
            std::exit(EXIT_FAILURE);
    }
}