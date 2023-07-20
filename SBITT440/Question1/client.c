#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define BUFFER_SIZE 1024

int clientSocket;

void signalHandling(int sig) {
    if (clientSocket > 0) {
        printf("\nDisconnecting from the server...\n");
        close(clientSocket);
    }
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, signalHandling);

    // Get server IP address and port number from the user
    char serverIP[16];
    printf("Enter server IP address: ");
    scanf("%s", serverIP);

    int serverPort;
    printf("Enter server port number: ");
    scanf("%d", &serverPort);

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Set up server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIP, &serverAddress.sin_addr) <= 0) {
        perror("Invalid server IP address");
        return EXIT_FAILURE;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        return EXIT_FAILURE;
    }

    printf("Connected to the server\n");

    while (1) {
        // Compose a string from user input
        char userInput[BUFFER_SIZE];
        printf("Enter a message: ");
        scanf(" %[^\n]", userInput);

        // Send user input string to the server
        if (send(clientSocket, userInput, strlen(userInput), 0) < 0) {
            perror("Sending data failed");
            return EXIT_FAILURE;
        }

        // Receive reply from the server
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead < 0) {
            perror("Receiving data failed");
            return EXIT_FAILURE;
        }

        printf("Server reply: %s\n", buffer);
    }

    return EXIT_SUCCESS;
}
