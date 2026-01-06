#include "connection.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int Connection::connect_to(const std::string& ip, uint16_t port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    connect(sockfd, (sockaddr*)&addr, sizeof(addr));
    return sockfd;
}

int Connection::listen_on(uint16_t port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 16);
    return server_fd;
}

int Connection::accept_client(int server_fd) {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    return accept(server_fd, (struct sockaddr *) &clientAddr, &clientLen);
}