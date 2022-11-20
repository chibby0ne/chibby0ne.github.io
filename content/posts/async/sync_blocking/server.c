#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_MESSAGE_SIZE 1024

void die(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{

    struct addrinfo *res;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int errcode = getaddrinfo("127.0.0.1", "1337", &hints, &res);
    if (errcode != 0) {
        gai_strerror(errcode);
    }

    // Creates a socket of family UNIX and type stream
    int fd = socket(hints.ai_family, hints.ai_socktype, 0);
    if (fd == -1) {
        die("Could not open socket");
    }

    // Bind the socket to the port
    errcode = bind(fd, res->ai_addr, res->ai_addrlen);
    if (errcode == -1) {
        die("could not bind the socket to the address");
    }

    // Listen to the connection from the client
    errcode = listen(fd, 10);
    if (errcode == -1) {
        die("could not listen to the socket");
    }

    // Accept the incoming connection
    struct sockaddr client_addr;
    socklen_t size_client_addr =  sizeof(client_addr);
    int new_fd = accept(fd, &client_addr, &size_client_addr);
    if (new_fd == -1) {
        die("error accepting the connection");
    }

    // First we receive since that's what the client is doing first
    char received_message[MAX_MESSAGE_SIZE];
    memset(received_message, 0, MAX_MESSAGE_SIZE);
    errcode = recv(new_fd, received_message, MAX_MESSAGE_SIZE, 0);
    if (errcode == -1) {
        die("Could not receive from client");
    }
    printf("Received from client: \"%s\"\n", received_message);

    // Then we send a message from stdin
    char message[MAX_MESSAGE_SIZE];
    if (fgets(message, MAX_MESSAGE_SIZE, stdin) == NULL) {
        die("could not read from stdin");
    }
    message[strcspn(message, "\r\n")] = 0;
    printf("Entered: \"%s\"\n", message);
    errcode = send(new_fd, message, strlen(message), 0);
    if (errcode == -1) {
        die("Could not send message to client");
    }
    printf("Goodbye!\n");
    freeaddrinfo(res);
    return 0;
}
