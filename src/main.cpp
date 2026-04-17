#include <fstream>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

std::mutex m;

void log(const std::string& method, const std::string& path, const std::string& status) {
    std::lock_guard<std::mutex> guard(m);

    std::cout << "[Thread " << std::this_thread::get_id() << "] " << method << ' ' << path << " -> " << status << '\n';
}

bool checkFile(const std::string& path, const std::string& base, std::string& body, std::string& content_type) {
    if (!path.empty()) {
            std::string file_path = path;
            file_path.erase(0, 1);
            file_path.insert(0, base);
            
            std::ifstream file(file_path);

            if (file.is_open()) {
                size_t lastPos = file_path.find_last_of('.');

                if (lastPos != std::string::npos) {
                    std::string content_check = file_path.substr(lastPos + 1);

                    if (content_check == "html") content_type = "text/html";
                    else if (content_check == "css") content_type = "text/css";
                }

                std::string line;

                while (std::getline(file, line)) {
                    body += line + "\r\n";
                }

                file.close();

                return true;
            }
    }

    return false;
}

void handle_client(int client_fd) {
    char buffer[1024] = {0};
    read(client_fd, buffer, 1024);

    std::string request(buffer);

    std::string first_line = request.substr(0, request.find("\r\n"));

    int first_space = first_line.find(" ");
    int second_space = first_line.find(" ", first_space + 1);

    std::string method = first_line.substr(0, first_space);
    std::string path = first_line.substr(first_space + 1, second_space - first_space - 1);

    std::string status;
    std::string body;
    std::string content_type = "text/plain";

    std::string base = "../public/";

    std::string effective_path = path;

    if (path == "/") {
        effective_path = "/index.html";
    }

    if (path == "/hello") {
        status = "200 OK";
        body = "Hello, World!";
    } else if (checkFile(effective_path, base, body, content_type)) {
        status = "200 OK";
    } else {
        status = "404 Not Found";
        body = "404 Not Found";
    }

    size_t content_length = body.size();

    std::string response = "HTTP/1.1 ";
    response += status;
    response += "\r\n";
    response += "Content-Type: ";
    response += content_type + "\r\n";
    response += "Content-Length: ";
    response += std::to_string(content_length) + "\r\n";
    response += "\r\n";
    response += body;
    
    write(client_fd, response.c_str(), response.size());

    log(method, path, status);

    close(client_fd);
}

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

        std::thread t(handle_client, client_fd);
        t.detach();
    }

    close(server_fd);

    return 0;
}