#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1; // do not assume the port is 80, since it could be set to something else
    }
    int port = atoi(argv[1]);
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    char message[1024];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // Port number
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return 1;
    }

    printf("Connected to the server.\n");

    while(message[0] != 'q') {
    printf("type 'q' to quit\n");

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);


    send(clientSocket, message, strlen(message), 0);
    printf("Message sent: %s\n", message);

    // Receive a response from the server
    int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Received from server: %s\n", buffer);
    }
    } //endwhile

    // Close the socket
    close(clientSocket);
    return 0;
}