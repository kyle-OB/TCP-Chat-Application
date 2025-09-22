# Multi-Client TCP Chat Application (C++)

This is a real-time chat system built in C++ using TCP sockets and POSIX threads.  
The server can handle multiple clients concurrently, and clients can specify the connection port via command-line arguments.

## Features
- Multi-threaded server for multiple clients
- Real-time messaging
- Graceful disconnect handling
- Command-line port selection

## Build Instructions
```bash
g++ -o tcpServer tcpServer.cpp -lpthread
g++ -o tcpClient tcpClient.cpp
