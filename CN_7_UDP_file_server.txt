#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(sock);
        return -1;
    }

    std::cout << "Waiting for file..." << std::endl;

    std::ofstream outfile("received_file.txt", std::ios::binary);
    if (!outfile) {
        std::cerr << "Failed to create file" << std::endl;
        close(sock);
        return -1;
    }

    while (true) {
        int bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0) {
            std::cerr << "Error receiving data" << std::endl;
            break;
        }
        if (bytes_received == 0) {
            break;
        }
        outfile.write(buffer, bytes_received);
    }

    std::cout << "File received successfully!" << std::endl;

    outfile.close();
    close(sock);
    return 0;
}
