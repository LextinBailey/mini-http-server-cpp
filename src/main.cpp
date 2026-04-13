#include <iostream>
#include <netinet/in.h>
#include <string>
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

    while(true) {
        int client_fd = accept(server_fd, nullptr, nullptr);

        if (client_fd < 0) {
            std::cerr << "accept() failed.";
            continue;
        }

        char buffer[1024] = {0};
        read(client_fd, buffer, 1024);

        std::string request(buffer);

        std::string first_line = request.substr(0, request.find("\r\n"));

        int first_space = first_line.find(" ");
        int second_space = first_line.find(" ", first_space + 1);

        std::string path = first_line.substr(first_space + 1, second_space - first_space - 1);

        std::string status;
        std::string body;

        if (path == "/") {
            status = "200 OK";
            body = "Welcome!";
        } else if (path == "/hello") {
            status = "200 OK";
            body = "Hello, World!";
        } else {
            status = "404 Not Found";
            body = "404 Not Found";
        }

        std::string response = "HTTP/1.1 ";
        response += status;
        response += "\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "\r\n";
        response += body;
        
        write(client_fd, response.c_str(), response.size());

        close(client_fd);

    }

    close(server_fd);

    return 0;
}