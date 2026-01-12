#include "message.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>

Message::Message(const Message& other_msg) 
    : max_payload_size(other_msg.max_payload_size),
    msg_id_(other_msg.msg_id_),
    length_(other_msg.length_),
    payload_(other_msg.payload_) {
}

int Message::write_new_msg(uint32_t new_msg_id, const char* new_payload, 
    uint64_t new_length
) {
    if (new_length > max_payload_size) {
        return -1;
    }

    msg_id_ = new_msg_id;
    length_ = new_length;

    memcpy(payload_.data(), new_payload, new_length);
    return 0;
}

Message Message::pop_msg() {
    Message copy(*this);

    // No need to clear the data in this Message due to future overwrite
    return copy;
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