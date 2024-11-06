#include <iostream>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

#define BUF_SIZE 1024

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[BUF_SIZE];
    int serverAddrSize = sizeof(serverAddr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Get the filename to request
    cout << "Enter the filename to receive: ";
    string filename;
    cin >> filename;

    // Send filename to the server
    sendto(clientSocket, filename.c_str(), filename.length(), 0, (sockaddr*)&serverAddr, serverAddrSize);

    // Receive server response
    int bytesReceived = recvfrom(clientSocket, buffer, BUF_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
    buffer[bytesReceived] = '\0';  // Null-terminate the received data

    if (string(buffer) == "File found, sending...") {
        cout << "File found, receiving..." << endl;

        // Receive the file in chunks
        ofstream file("received_" + filename, ios::binary);
        while (true) {
            bytesReceived = recvfrom(clientSocket, buffer, BUF_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
            if (bytesReceived == 0) break;  // No more data

            file.write(buffer, bytesReceived);
        }

        cout << "File received successfully." << endl;
        file.close();
    } else {
        cout << "Error: " << buffer << endl;
    }

    // Clean up
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
