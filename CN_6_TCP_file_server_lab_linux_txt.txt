
//CLIENT

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    // Open the file to be sent
    std::ifstream infile("file_to_send.txt");
    if (!infile) {
        std::cerr << "Could not open the file!" << std::endl;
        return -1;
    }

    // Send the file
    while (infile.good()) {
        infile.read(buffer, BUFFER_SIZE);
        std::streamsize bytes_read = infile.gcount();
        send(sock, buffer, bytes_read, 0);
    }

    std::cout << "File sent successfully!" << std::endl;

    // Clean up
    infile.close();
    close(sock);
    return 0;
}