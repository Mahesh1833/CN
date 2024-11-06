#include <iostream>
#include <fstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Link with Ws2_32.lib for Windows Sockets

#define PORT 8080

int main() {
    WSADATA wsaData;
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return -1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or Address not supported" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Open the file to send
    std::ifstream infile("file_to_send.txt", std::ios::binary);
    if (!infile) {
        std::cerr << "Failed to open file" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Send the file contents
    char buffer[1024];
    while (infile.read(buffer, sizeof(buffer)) || infile.gcount() > 0) {
        send(sock, buffer, infile.gcount(), 0);
    }

    std::cout << "File sent successfully!" << std::endl;

    // Close file and socket, cleanup Winsock
    infile.close();
    closesocket(sock);
    WSACleanup();
    return 0;
}
