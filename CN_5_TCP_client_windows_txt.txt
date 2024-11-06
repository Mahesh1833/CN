// TCP Client in C++ (Windows-compatible)
#include <iostream>
#include <cstring>
#include <winsock2.h>    // Windows Sockets API
#include <ws2tcpip.h>    // For inet_pton, additional socket functions

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib for Windows Sockets

#define PORT 8080

int main() {
    WSADATA wsaData;
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *hello = "Hello";

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return -1;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or Address not supported" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Send message to server
    send(sock, hello, strlen(hello), 0);
    std::cout << "Hello message sent to server" << std::endl;

    // Close socket
    closesocket(sock);
    WSACleanup();

    return 0;
}
