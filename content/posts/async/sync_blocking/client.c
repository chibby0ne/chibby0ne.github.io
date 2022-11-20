#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>

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

    errcode = connect(fd, res->ai_addr, res->ai_addrlen);
    if (errcode == -1) {
        die("could not connect to address");
    }

    char message[MAX_MESSAGE_SIZE];
    printf("Enter the message to send to server:\n");
    if (fgets(message, MAX_MESSAGE_SIZE, stdin) == NULL) {
        die("Could not read from stdin");
    }
    // Remove the potential newline
    message[strcspn(message, "\r\n")] = 0;
    printf("Entered: \"%s\"\n", message);
    errcode = send(fd, message, strlen(message), 0);
    if (errcode == -1) {
        die("Could not send message");
    }


    char received_message[MAX_MESSAGE_SIZE];
    memset(received_message, 0, MAX_MESSAGE_SIZE);
    errcode = recv(fd, received_message, MAX_MESSAGE_SIZE, 0);
    if (errcode == -1) {
        die("Could not receive message");
    }

    printf("Received: \"%s\"\n", received_message);
    printf("Goodbye!\n");
    freeaddrinfo(res);
    return 0;
}

