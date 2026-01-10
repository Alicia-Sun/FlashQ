#pragma once

#include <stdint.h>

#include <string>

class Connection{
    public:
        static int connect_to(const std::string& ip, uint16_t port);
        static int listen_on(uint16_t port);
        static int accept_client(int server_fd);

};