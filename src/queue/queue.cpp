#include "queue.h"

#include <string.h>
#include <sys/socket.h>

#include <iostream>

#include "protocol_constants.h"
#include "../utils/helpers.h"

using namespace std;

int FlashQ::init(int max_capacity, uint64_t max_msg_size,
    std::string config_file
) {
    bool success = parse_config(config_file, server_config);
    if (!success) { return -1; }

    max_payload_size = max_msg_size;
    head_.store(0);
    tail_.store(0);
    capacity_ = max_capacity;
    mask_ = capacity_ - 1;

    queue_.reserve(capacity_);
    sequence_ = std::make_unique<std::atomic<size_t>[]>(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        queue_.emplace_back(max_msg_size);
        sequence_[i].store(i, std::memory_order_relaxed);
    }

    cout << "Initializing New Server..." << endl;
    cout << "IP Address: " << server_config.ip_address << endl;
    cout << "Port Number: " << server_config.port << endl;
    cout << "Queue Capacity: " << queue_.size() << endl;

    return 0;
}

int FlashQ::enqueue(int msg_id, const char* data, size_t data_len) {
    size_t pos = tail_.fetch_add(1);
    int slot = pos & mask_;

    // Only ready to be written by producer when tail_ == sequence_[slot]
    while (sequence_[slot].load(std::memory_order_relaxed) != pos) {
        // spin
    }
    queue_[slot].write_new_paylaod(msg_id, data, data_len);

    // Publish the element
    sequence_[slot].store(pos + 1);
    return 0;
}

// TODO
Message FlashQ::dequeue() {
}

// TODO
void FlashQ::handle_client(int clientfd) {
    const char *greeting = "+OK Server Ready\n";
    send(clientfd, greeting, strlen(greeting), MSG_NOSIGNAL | MSG_DONTWAIT);

    const size_t BUFFER_SIZE = max_payload_size;
    char buffer[BUFFER_SIZE];
    size_t buf_len = 0; // currently how much of the buffer is filled

    while (true) {
        // reset the buffer if nearly full
        if (buf_len >= BUFFER_SIZE - 1) {
            perror("Error: Input is too large causing buffer overflow, resetting...");
            buf_len = 0;
        }

        // Read socket contents
        ssize_t bytes_read = recv(clientfd, buffer + buf_len, BUFFER_SIZE - buf_len - 1, 0);
        if (bytes_read < 0) {
            perror("recv() failed");
            break;
        } else if (bytes_read == 0) {
            perror("Connection closed by peer");
            break;
        }

        buf_len += bytes_read;
        buffer[buf_len] = '\0'; // null terminate

        // Begin processing commands
        char* line_start = buffer;
        char* line_end;

        // Treat each <CRLF> as end of command
        while ((line_end = strstr(line_start, "\r\n")) != nullptr) {
            *line_end = '\0'; // null-terminate line
            fprintf(stderr, "[%d] Line received: '%s'\n", clientfd, line_start);

            // Command Parsing Stubs
            if (strncasecmp(line_start, CMD_ENQUEUE, strlen(CMD_ENQUEUE)) == 0) {
                // TODO: handle ENQUEUE
            }
            else if (strncasecmp(line_start, CMD_BATCH_ENQUEUE, strlen(CMD_BATCH_ENQUEUE)) == 0) {
                // TODO: handle BATCH_ENQUEUE
            }
            else if (strncasecmp(line_start, CMD_DEQUEUE, strlen(CMD_DEQUEUE)) == 0) {
                // TODO: handle DEQUEUE
            }
            else if (strncasecmp(line_start, CMD_DEQUEUE_FAST, strlen(CMD_DEQUEUE_FAST)) == 0) {
                // TODO: handle DEQUEUE_FAST
            }
            else if (strncasecmp(line_start, CMD_ACK, strlen(CMD_ACK)) == 0) {
                // TODO: handle ACK
            }
            else {
                fprintf(stderr, "[%d] Unknown command: '%s'\n", clientfd, line_start);
            }

            line_start = line_end + 2; // skip past \r\n
        }

        // Move leftover partial data of new command to beginning of buffer
        size_t leftover = buf_len - (line_start - buffer);
        memmove(buffer, line_start, leftover);
        buf_len = leftover;
    }
}