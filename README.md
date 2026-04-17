# Mini HTTP Server (C++)

A mini HTTP server built from scratch using C++ and low-level POSIX sockets.

## 📝 Overview

Mini HTTP Server (C++) is a lightweight HTTP server built from scratch using low-level POSIX sockets. It handles TCP connections, parses HTTP requests, implements basic routing, and responds with properly formatted HTTP responses.

## ⚡️ Quick Example

```bash
$ curl localhost:8080/hello
```
```bash
Hello, World!
```

## 🔥 Features

### 🛠️ Core Features

- [x] Accept TCP connections
- [x] Parse HTTP request line (method, path, version)
- [x] Basic routing system (`/`, `/hello`)
- [x] Return valid HTTP responses with status codes
- [x] Handle multiple clients (thread-per-connection)
- [x] Basic multithreading
- [x] Thread-safe logging
- [x] Serve static files

### 👨‍💻 Developer Experience

- [x] Error messages for failed TCP connections

### 🧱 Planned

- [ ] Upgrade routing system (`/users`, `/about`)

## 💡 What This Demonstrates

This project demonstrates:
- Networking (sockets)
- Request parsing
- Response formatting (simple HTTP)
- Ability to handle multiple concerns (routing, parsing, responding)
- File I/O

## ⚙️ Build and Usage

### Requirements

C++17 or later

### Build with CMake

```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

### Start the Server

```bash
$ ./server
```

### Testing the Server

```bash
$ curl localhost:8080/
$ curl localhost:8080/hello
$ curl localhost:8080/invalid
```

## 🧠 Design Highlights

- Clear separation between entry (`main.cpp`), networking (`server.cpp`), and HTTP logic (`http.cpp`)
- `log()` and `check_file()` encapsulated inside anonymous namespace

## ⚙️ How It Works Internally

⚠️ This section is optional and provides a deeper look into the internal design.

### 1. Socket Creation

Created a socket for the server so the program can send and receive data over a network.

`socket()` returns a file descriptor to represent the socket.

- IPv4 addresses
- TCP communication type

### 2. Configure the Address

Used `sockaddr_in` to configure an IP address and a port to bind to the socket.

- Port: 8080
- Currently accepts connections from any IP

### 3. Bind the Address and Port to the Socket

`bind()` assigned the IP address and port to the socket, so the OS knows where to direct incoming connections.

### 4. Listen for Incoming Connections

`listen()` allows the socket to accept incoming connection requests and queue them.

Currently uses the system's maximum queue size.

### 5. Accepting Clients

`accept()` blocks until a connection request arrives.

Once a client connects, it returns a file descriptor for the client connection.

### 6. Read and Store Data

Read the data sent from the client.

Store the data in a buffer.

### 7. Routing

Find the path in the data and respond accordingly.

- `200`: Accepted route
- `404 Not Found`: Route does not exist

### 8. Multithreading

When a client connects:
- `accept()` returns `client_fd`
- Client handed off to a thread that calls `handle_client`
- Main thread accepts more clients

Threads are detached to allow independent execution without blocking the main thread.

🔄 Tradeoffs of thread-per-connection:
- ✅ Benefits:
    - Simple
    - Non-blocking
- ❌ Downsides:
    - No control over lifetime
    - Potential resource exhaustion

Future improvement: Replace thread-per-connection with a thread pool or event-driven model for better scalability.

### 9. Thread-safe Logging

`log()` wraps logging output with a global mutex.

Protects multiple threads sharing `stdout` from race conditions.

Each thread must wait to acquire the lock before logging.

Logged:
- Thread ID
- Method
- Path
- Status

Example:

```bash
[Thread 0x16b7eb000] GET / -> 200 OK
```

### 10. Serving Static Files

`check_file()` interprets file paths, opens the file, reads its contents, and returns it as the HTTP response body with the correct Content-Type.

Example:

```bash
$ curl localhost:8080/index.html
```

```bash
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Test Title</title>
</head>
<body>
    <h1>Test</h1>
    <p>This is a paragraph of text.</p>
</body>
</html>
```

### Example Request

```bash
GET /hello HTTP/1.1
Host: localhost:8080
```

## 📁 Project Structure

```
mini-http-server-cpp/
├── CMakeLists.txt
├── public/
│   └── index.html
├── include/
│   ├── http.h
│   └── server.h
├── src
│   ├── http.cpp
│   ├── main.cpp
│   └── server.cpp
├── progress-log.md
└── README.md
```

## 📈 Development Notes

Progress and development insights are tracked in `progress-log.md`.