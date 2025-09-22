// g++ -o tcpServer tcpServer.cpp
// ./tcpServer Server
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <vector>
#define MAX 80
#define PORT 54550
#define SA struct sockaddr
// Mutex for synchronizing shared resources
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Function designed for chat between client and server
void* handleClient(void* arg) {
int connfd = *((int*)arg);
free(arg);
char buff[MAX];
for (;;) {
bzero(buff, MAX);
// Read message from client
read(connfd, buff, sizeof(buff));
printf("From client: %s\n", buff);
// Check if client wants to exit
if (strncmp("exit", buff, 4) == 0) {
printf("Client has disconnected.\n");
break;
}
// Echo message back to client
write(connfd, buff, sizeof(buff));
}
close(connfd);
pthread_exit(NULL);
}
int main(int argc, char* argv[]) {
int sockfd, connfd, len;
struct sockaddr_in servaddr, cli;
socklen_t addr_size;
std::vector<pthread_t> threads;
// Socket creation and verification
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd == -1) {
printf("Socket creation failed...\n");
exit(0);
} else {
printf("Socket successfully created..\n");
}
bzero(&servaddr, sizeof(servaddr));
// Assign IP, PORT
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
servaddr.sin_port = htons(PORT);
// Binding newly created socket to given IP and verification
if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
printf("Socket bind failed...\n");
exit(0);
} else {
printf("Socket successfully binded..\n");
}
// Now server is ready to listen and verification
if ((listen(sockfd, 5)) != 0) {
printf("Listen failed...\n");
exit(0);
} else {
printf("Server listening..\n");
}
addr_size = sizeof(cli);
// Infinite loop to accept multiple client connections
while (1) {
connfd = accept(sockfd, (SA*)&cli, &addr_size);
if (connfd < 0) {
printf("Server accept failed...\n");
exit(0);
} else {
printf("New client connected.\n");
// Create a new thread for each client
pthread_t thread_id;
int* client_sock = (int*)malloc(sizeof(int));
*client_sock = connfd;
pthread_create(&thread_id, NULL, handleClient, client_sock);
threads.push_back(thread_id);
}
}
// Close the socket
close(sockfd);
return 0;
}
