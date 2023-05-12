#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define PORT 4200
#define MESSAGE_LENGTH 1024
#define BACKLOG 1               // Size of the server-side connection queue

int main() 
{
    // Create a Socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket() failed");
        return errno;
    }
    printf("Socket created successfully!\n");

    // Bind the Socket to an address
    struct sockaddr_in server_addr = { 0 };
    server_addr.sin_family = AF_INET;   
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); 
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind() failed");
        close(server_socket);
        return errno;
    }
    printf("Socket bound to address successfully!\n");

    // Start Listening
    if (listen(server_socket, BACKLOG) == -1) {
        perror("listen() failed");
        close(server_socket);
        return errno;
    }
    printf("Socket listening on port %d\n", PORT);

    // Accept Connection Request from Client
    struct sockaddr_in client_addr = { 0 };
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("accept() failed");
        close(server_socket);
        return errno;
    }
    printf("Accepted client connection!\n");

    // Receive Message from Client
    char client_message[MESSAGE_LENGTH] = { 0 };
    ssize_t recv_result = recv(client_socket, client_message, sizeof(client_message), 0);
    if (recv_result == -1) {
        perror("recv() failed");
        close(client_socket);
        close(server_socket);
        return errno;
    }
    printf("Received message from client: %s\n", client_message);

    // Close Sockets and Exit
    close(client_socket);
    close(server_socket);
    printf("Closing sockets successful!\n");
    return 0;
}
