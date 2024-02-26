#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

const int PORT = 8080;

int main() {
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char buffer[30000] = {0};
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello Word!</h1></body></html>";

    // Creazione della socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket creation");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // Associazione della socket all'indirizzo e alla porta specificati
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    // Ascolto di connessioni in entrata
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while(true) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, buffer, 30000);
        std::cout << "----- Request -----\n" << buffer << std::endl;
        write(new_socket, hello, strlen(hello));
        std::cout << "----- Response sent -----\n";
        close(new_socket);
    }
    return 0;
}
