// g++ -o tcpClient tcpClient.cpp
// ./tcpClient <name> <port>

#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 80
#define SA struct sockaddr

void openChat(int sockfd, char* name) {
    char buff[MAX];
    int n;

    for (;;) {
        bzero(buff, sizeof(buff));
        printf("%s: ", name);
        n = 0;

        // Read message from user
        while ((buff[n++] = getchar()) != '\n')
            ;

        // Send to server
        write(sockfd, buff, sizeof(buff));

        // Receive from server
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server: %s", buff);

        // Check for exit
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: ./tcpClient <name> <port>\n");
        return 1;
    }

    int sockfd;
    struct sockaddr_in servaddr;
    char* name = argv[1];
    int port = atoi(argv[2]);

    // Socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);

    // Connect client to server
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    } else {
        printf("Connected to the server..\n");
    }

    // Open chat
    openChat(sockfd, name);

    // Close socket
    close(sockfd);
    return 0;
}
