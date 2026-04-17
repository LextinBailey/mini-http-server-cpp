#include <iostream>
#include <thread>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "http.h"
#include "server.h"

void start_server(int port) {
    std::cout << "Mini HTTP Server starting...\n";

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        std::cerr << "socket() failed.";
        return;
    }

    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "bind() failed.";
        return;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        std::cerr << "listen() failed.";
        return;
    }

    while(true) {
        int client_fd = accept(server_fd, nullptr, nullptr);

        if (client_fd < 0) {
            std::cerr << "accept() failed.";
            continue;
        }

        std::thread t(handle_http, client_fd);
        t.detach();
    }

    close(server_fd);
}