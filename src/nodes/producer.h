#pragma once
#include <string>
#include <vector>

class ProducerNode {
    public:
        // Initializer
        void init(std::string config_file);

        // Client handler
        void handle_client(int clientfd);

    private:
        int id;
        int port;
        std::string ip;
        int listener_socketfd;

        int primary_queue_fd;
};