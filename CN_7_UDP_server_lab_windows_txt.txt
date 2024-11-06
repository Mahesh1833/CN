#include <iostream>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

#define BUF_SIZE 1024

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    char buffer[BUF_SIZE];
    int clientAddrSize = sizeof(clientAddr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port 12345..." << endl;

    while (true) {
        // Receive the filename request from client
        int bytesReceived = recvfrom(serverSocket, buffer, BUF_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "Receive failed" << endl;
            continue;
        }

        buffer[bytesReceived] = '\0';  // Null-terminate the received data
        string filename(buffer);

        // Check if the file exists
        ifstream file(filename, ios::binary);
        if (!file) {
            const char* errorMsg = "File not found";
            sendto(serverSocket, errorMsg, strlen(errorMsg), 0, (sockaddr*)&clientAddr, clientAddrSize);
        } else {
            const char* successMsg = "File found, sending...";
            sendto(serverSocket, successMsg, strlen(successMsg), 0, (sockaddr*)&clientAddr, clientAddrSize);

            // Send the file in chunks
            while (file.read(buffer, BUF_SIZE)) {
                sendto(serverSocket, buffer, file.gcount(), 0, (sockaddr*)&clientAddr, clientAddrSize);
            }
            if (file.gcount() > 0) {
                sendto(serverSocket, buffer, file.gcount(), 0, (sockaddr*)&clientAddr, clientAddrSize);
            }
            file.close();
            cout << "File sent to client." << endl;
        }
    }

    // Clean up
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
