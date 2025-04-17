#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

int loop = 1; // Left over from threading attempt, would be a normal var otherwise

void handleConnection(int clientSocket) {
    char buffer[1024];
    while (loop) {
    int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; 
        printf("Received: %s\n", buffer);
        write(clientSocket, buffer, bytesRead); 
        if(buffer[0] == 'q') {
            printf("Client typed a 'q'. Closing server.\n");
            loop = 0;
        }
    } else if (bytesRead == 0) {
        printf("Client disconnected.\n");
        break; // Client disconnected
    } 
    }
}

int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc != 2) {
        fprintf(stderr, "Usage to use a port other than 8080: %s <port>\n", argv[0]);
        port = 80;
    }
    else{
        port = atoi(argv[1]);
    }
    int serverSocket, clientSocket; // server and client socket
    struct sockaddr_in serverAddr, clientAddr; // server and client address
    socklen_t addr_size = sizeof(clientAddr); // size of the address 

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port); 

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        close(serverSocket);
        return 1;
    }

    printf("Server is listening on port %d...\n", port);

    while (loop) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }
        //pthread_t tid; // threading the connection
        //pthread_create(&tid, NULL, &handleConnection, &clientSocket);
        //pthread_join(tid, NULL);
        handleConnection(clientSocket); // gave up on threading

        close(clientSocket);
    }
    //pthread_exit(NULL);

    close(serverSocket); 
    return 0;
}