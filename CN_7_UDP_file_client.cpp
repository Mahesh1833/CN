#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return -1;
    }

    std::ifstream infile("file_to_send.txt", std::ios::binary);
    if (!infile) {
        std::cerr << "Failed to open file" << std::endl;
        close(sock);
        return -1;
    }

    while (infile.read(buffer, BUFFER_SIZE) || infile.gcount() > 0) {
        int bytes_to_send = infile.gcount();
        sendto(sock, buffer, bytes_to_send, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }

    std::cout << "File sent successfully!" << std::endl;

    infile.close();
    close(sock);
    return 0;
}
