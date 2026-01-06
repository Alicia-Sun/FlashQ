#include "queue.h"
#include "protocol_constants.h"
#include "../utils/helpers.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

// QueueNode is a server instance that acts as a message queue in a distributed system
int QueueNode::init(int node_id, size_t max_capacity, uint64_t max_msg_size, std::string config_file) {
    bool success = parse_config(node_id, config_file, server_configs);
    if (!success) {
        return -1;
    }

    id = node_id;
    is_primary = server_configs[node_id].is_primary;
    max_payload_size = max_msg_size;

    head.store(0);
    tail.store(0);
    capacity = max_capacity;

    slab.reserve(capacity);
    queue.reserve(capacity);
    sequence = std::make_unique<std::atomic<size_t>[]>(capacity);
    for (size_t i = 0; i < capacity; ++i) {
        slab.emplace_back(max_msg_size);
        queue.push_back(&slab[i]);
        sequence[i].store(i, std::memory_order_relaxed);
    }

    cout << "Initializing New Server..." << endl;
    cout << "ID: " << node_id << endl;
    cout << "IP Address: " << server_configs[node_id].ip_address << endl;
    cout << "Port Number: " << server_configs[node_id].port << endl;
    cout << "Primary Status: " << std::boolalpha << server_configs[node_id].is_primary << endl;
    cout << "Queue Capacity: " << queue.size() << endl;

    return 0;
}

int QueueNode::enqueue(int msg_id, const char* data, size_t data_len) {
    // TODO
    while (true) {
        size_t pos = tail.load(std::memory_order_relaxed);
        size_t index = pos % capacity;

        size_t seq = sequence[index].load(std::memory_order_acquire);
        intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);

        if (diff == 0) {
            // slot is available to write
        } else if (diff > 0) {
            // queue is full. waiting for dequeue
        } else {
            // slot is being contested by another enqueue, must retry
        }
    }
}

Message QueueNode::dequeue() {
    // TODO
}

uint32_t QueueNode::dequeue_by_ptr(Message*& out_msg_ptr) {
    // TODO
}

void QueueNode::handle_client(int clientfd) {
    // TODO
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