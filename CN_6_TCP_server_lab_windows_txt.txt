#include <iostream>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void file_server(const char* host, int port) {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host);
    serverAddr.sin_port = htons(port);

    // Bind the socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        return;
    }

    // Listen for incoming connections
    listen(serverSocket, 1);
    std::cout << "Server is listening on " << host << ":" << port << std::endl;

    // Accept incoming connection
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed!" << std::endl;
        closesocket(serverSocket);
        return;
    }

    // Receive file name
    char fileName[BUFFER_SIZE];
    recv(clientSocket, fileName, sizeof(fileName), 0);
    std::cout << "Receiving file: " << fileName << std::endl;

    // Open file to write
    std::ofstream outputFile(fileName, std::ios::binary);

    // Receive file data in chunks
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        outputFile.write(buffer, bytesReceived);
    }

    std::cout << "File received successfully!" << std::endl;

    // Clean up
    outputFile.close();
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    const char* host = "127.0.0.1";  // Server address
    int port = 12345;  // Server port
    file_server(host, port);
    return 0;
}
