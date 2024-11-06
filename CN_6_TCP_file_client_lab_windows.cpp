#include <iostream>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void file_client(const char* server_host, int server_port, const char* file_name) {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(server_host);
    serverAddr.sin_port = htons(server_port);

    // Connect to the server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        return;
    }

    // Send file name
    send(clientSocket, file_name, strlen(file_name), 0);
    std::cout << "Sending file: " << file_name << std::endl;

    // Open the file to read
    std::ifstream inputFile(file_name, std::ios::binary);
    if (!inputFile) {
        std::cerr << "File not found!" << std::endl;
        closesocket(clientSocket);
        return;
    }

    // Send file data in chunks
    char buffer[BUFFER_SIZE];
    while (inputFile.read(buffer, sizeof(buffer))) {
        send(clientSocket, buffer, inputFile.gcount(), 0);
    }
    if (inputFile.gcount() > 0) {
        send(clientSocket, buffer, inputFile.gcount(), 0);
    }

    std::cout << "File sent successfully!" << std::endl;

    // Clean up
    inputFile.close();
    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    const char* server_host = "127.0.0.1";  // Server address
    int server_port = 12345;  // Server port
    const char* file_name = "sample.txt";  // File to send
    file_client(server_host, server_port, file_name);
    return 0;
}
