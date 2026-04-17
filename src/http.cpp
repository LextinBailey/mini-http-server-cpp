#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include <unistd.h>

#include "http.h"

std::mutex m;

namespace {
    void log(const std::string& method, const std::string& path, const std::string& status) {
        std::lock_guard<std::mutex> guard(m);

        std::cout << "[Thread " << std::this_thread::get_id() << "] " << method << ' ' << path << " -> " << status << '\n';
    }

    bool check_file(const std::string& path, const std::string& base, std::string& body, std::string& content_type) {
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
}

void handle_http(int client_fd) {
    char buffer[1024] = {0};
    ssize_t bytes_read = read(client_fd, buffer, 1024);

    if (bytes_read < 0) {
        log("ERROR", "read() failed", "400 Bad Request");
        close(client_fd);
        return;
    } else if (bytes_read == 0) {
        close(client_fd);
        return;
    }

    std::string request(buffer, bytes_read);

    std::string first_line = request.substr(0, request.find("\r\n"));

    size_t first_space = first_line.find(" ");
    size_t second_space = first_line.find(" ", first_space + 1);

    std::string method = first_line.substr(0, first_space);
    std::string path = first_line.substr(first_space + 1, second_space - first_space - 1);

    std::string status;
    std::string body;
    std::string content_type = "text/plain";

    const std::string base = "../public/";

    std::string effective_path = path;

    if (path == "/") {
        effective_path = "/index.html";
    }

    if (path == "/hello") {
        status = "200 OK";
        body = "Hello, World!";
    } else if (check_file(effective_path, base, body, content_type)) {
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

