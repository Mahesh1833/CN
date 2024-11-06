
//SERVER

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket failed" << std::endl;
        return -1;
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "setsockopt failed" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }

    // Receive the file
    std::ofstream outfile("received_file");
    int bytes_received;
    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
   
         std::cout<<buffer;  
        outfile.write(buffer, bytes_received);
    }

    std::cout << "File received successfully!" << std::endl;

    // Clean up
    outfile.close();
    close(new_socket);
    close(server_fd);
    return 0;
}
