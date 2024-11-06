#include <iostream>
#include <fstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib for Windows Sockets

#define PORT 8080

int main() {
    WSADATA wsaData;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return -1;
    }

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return -1;
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        std::cerr << "Setsockopt error" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    std::cout << "Waiting for connections..." << std::endl;

    // Accept a client connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    std::cout << "Client connected!" << std::endl;

    // Open file to write received data
    std::ofstream outfile("received_file.txt", std::ios::binary);
    if (!outfile) {
        std::cerr << "Failed to create file" << std::endl;
        closesocket(new_socket);
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    // Receive file data from client
    int bytes_received;
    while ((bytes_received = recv(new_socket, buffer, sizeof(buffer), 0)) > 0) {
        outfile.write(buffer, bytes_received);
    }

    std::cout << "File received successfully!" << std::endl;

    // Close file and cleanup
    outfile.close();
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
