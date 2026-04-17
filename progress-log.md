# Progress Log

A day-by-day log of development decisions, features, and design evolution.

## Day 1 (Apr 12, 2026): GitHub Repo, Initial Structure, Socket Creation, Bind to Port, Listen, Accept, Read and Print

- Created GitHub repository
- Setup project structure
- Initial commit
- Implemented TCP server:
    - socket
    - bind
    - listen
    - accept connection
- Read raw HTTP request bytes
- Print them to the terminal

## Day 2 (Apr 13, 2026): Respond, Parsing, Routing, Status Code, Basic Multithreading

- Updated `README.md`
- Responded with a simple message: "Hello, World!"
- Parsed the HTTP request
- Finds the path
- Sends different bodies based on the path
- Sends appropriate status code
- Added support for multiple clients
- Basic multithreading
    - Thread-per-connection
    - Downsides of unbounded thread creation and resource overhead

## Day 3 (Apr 14, 2026): Polished README

- Improved project documentation clarity
- Added usage examples with curl
- Structured README for readability and quick understanding

## Day 4 (Apr 16, 2026): Accept Fix, Thread-safe Logging, Serve Static Files

- Fixed accept ordering
- Added thread-safe logging
    - Prevents race conditions
- Added static file serving
    - Tested with `public/index.html`
    - Aware of directory traversal vulnerabilities (future improvement)

## Day 5 (Apr 17, 2026): Refactor Design, Content-Length, Read Failure Check, Restructured Design, Malformed Request Handling, README Cleanup

- Refactored design to separate concerns between routes and files
    - Routes handled with `path`
    - Files handled with `effective_path`
- Added `Content-Length` in the HTTP response
    - Tells the client exactly how many bytes are in the response body
- Handled `read()` failure by logging an error and closing the connection
    - Was previously assuming read succeeded, full request arrived, and the buffer was valid
- Restructured design to separate key concerns
    - `main.cpp` = entry
    - `server.cpp` = networking only
    - `http.cpp` = HTTP logic only
- Guarded against if `\r\n` isn't found
- Updated all new changes to `README.md`

## 📌 Summary

This project evolved from a basic TCP server into a simple HTTP server capable of handling routing, static files, and multiple clients.


