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

## Day 2 (Apr 13, 2026): Respond, Parsing, Routing, Status Code, Multiple Clients

- Updated `README.md`
- Responded with a simple message: "Hello, World!"
- Parses the HTTP request
- Finds the path
- Sends different bodies based on the path
- Sends appropriate status code
- Added support for multiple clients