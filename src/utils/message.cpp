#include "message.h"
#include <cstring>

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