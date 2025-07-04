#include "queue.h"
#include "protocol_constants.h"
#include "../utils/helpers.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

int QueueNode::init(int id, size_t capacity, uint64_t max_msg_size, std::string config_file) {
    // TODO
    // must parse in config file and fill out fields
    // preallocate the n messages on the slab and set up the queue
    bool success = parse_config(id, config_file, server_configs);
    if (!success) {
        return -1;
    }

    cout << "Initializing New Server" << endl;
    cout << "ID: " << id << endl;
    cout << "IP Address: " << server_configs[id].ip_address << endl;
    cout << "Port Number: " << server_configs[id].port << endl;
    cout << "Primary Status: " << std::boolalpha << server_configs[id].is_primary << endl;

    max_payload_size = max_msg_size;
    for (size_t i = 0; i < capacity; ++i) {
        slab.push_back(Message(max_msg_size));
        queue.push_back(&slab[i]);
    }
    cout << "Queue Capacity: " << queue.size() << endl;

    front_ptr = -1;
    back_ptr = -1;

    return 0;
}

int QueueNode::enqueue(int msg_id, const char* data, size_t data_len) {
    // TODO
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

    // Some small extra space for the actual command entered
    const size_t BUFFER_SIZE = max_payload_size + 20;
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