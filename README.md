# Mini HTTP Server (C++)

A mini HTTP server built from scratch using C++ and low-level sockets.

## 🔥 Features

### 🛠️ Core Features

- [x] Accept TCP connections

### 👨‍💻 Developer Experience

- [x] Error messages for failed TCP connections

### 🧱 Planned

- [ ] Parse basic HTTP requests
- [ ] Return simple HTTP responses
- [ ] Handle routes

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

### 4. Wait for Incoming Connections

`listen()` allowed the socket to accept incoming connection requests and queue them.

Currently uses the system's maximum queue size.

### 5. Connect Clients

`accept()` blocks until a connection request arrives.

Once a client connects, it returns a file descriptor for the client connection.


## 📁 Project Structure

```
mini-http-server-cpp/
├── CMakeLists.txt
├── examples/
├── include/
├── src/
│   └── main.cpp
├── progress-log.md
└── README.md
```

## 🚧 Status

Under construction...

## 📈 Development Notes

Progress and development insights are tracked in `progress-log.md`.