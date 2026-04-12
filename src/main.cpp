#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    std::cout << "Mini HTTP Server starting...\n";

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        std::cerr << "socket() failed.";
        return 1;
    }

    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "bind() failed.";
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        std::cerr << "listen() failed.";
        return 1;
    }

    int client_fd = accept(server_fd, nullptr, nullptr);

    if (client_fd < 0) {
        std::cerr << "accept() failed.";
        return 1;
    }

    std::cout << "Client connected\n";

    return 0;
}