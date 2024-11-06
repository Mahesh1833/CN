#include <iostream>
#include <fstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return -1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::ifstream infile("file_to_send.txt", std::ios::binary);
    if (!infile) {
        std::cerr << "Failed to open file" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Send file data in chunks
    while (infile.read(buffer, BUFFER_SIZE) || infile.gcount() > 0) {
        int bytes_to_send = infile.gcount();
        if (sendto(sock, buffer, bytes_to_send, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            std::cerr << "Send failed" << std::endl;
            break;
        }
    }

    std::cout << "File sent successfully!" << std::endl;

    infile.close();
    closesocket(sock);
    WSACleanup();
    return 0;
}
