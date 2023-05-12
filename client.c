#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define PORT 4200
#define MESSAGE_LENGTH 1024

int main()
{
    // Create a Socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket() failed");
        return errno;
    }
    printf("Socket created successfully!\n");

    // Define the Server Address and Port
    struct sockaddr_in server_addr = { 0 };
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the Server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect() failed");
        close(client_socket);
        return errno;
    }
    printf("Connected to server successfully!\n");

    // Send a Message to the Server
    char message[MESSAGE_LENGTH] = {0};
    printf("Enter Message: ");
    if (fgets(message, MESSAGE_LENGTH, stdin) == NULL) {
        perror("fgets() failed");
        close(client_socket);
        return errno;
    }
    ssize_t sent_bytes = send(client_socket, message, strlen(message), 0);
    if (sent_bytes == -1) {
        perror("send() failed");
        close(client_socket);
        return errno;
    }
    printf("Message sent to the server: %s\n", message);  

    // Close the Socket and Exit
    close(client_socket);
    printf("Connection closed!\n");
    return 0;
}
