#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include "common.h"

void die(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

struct socket_config *configure_server(char *domain, char *service)
{
    struct socket_config *config = configure_client(domain, service);

    // Bind the socket to the port
    int errcode = bind(config->socket, config->res->ai_addr, config->res->ai_addrlen);
    if (errcode == -1) {
        die("could not bind the socket to the address");
    }

    // Listen to the connection from the client
    errcode = listen(config->socket, 10);
    if (errcode == -1) {
        die("could not listen to the socket");
    }

    return config;
}

int accept_connection(struct socket_config *config)
{
    struct sockaddr client_addr;
    socklen_t size_client_addr =  sizeof(client_addr);
    int new_fd = accept(config->socket, &client_addr, &size_client_addr);
    if (new_fd == -1) {
        die("error accepting the connection");
    }
    return new_fd;
}

struct socket_config *configure_client(char *domain, char *service)
{

    struct addrinfo *res;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int errcode = getaddrinfo(domain, service, &hints, &res);
    if (errcode != 0) {
        gai_strerror(errcode);
    }

    // Creates a socket of family UNIX and type stream
    int fd = socket(hints.ai_family, hints.ai_socktype, 0);
    if (fd == -1) {
        die("Could not open socket");
    }

    struct socket_config *config = (struct socket_config *) malloc(sizeof(struct socket_config));
    if (config == NULL) {
        die("Could not allocate socket_config");
    }
    config->socket = fd;
    config->res = res;
    return config;
}

void connect_client(struct socket_config *config)
{
    int errcode = connect(config->socket, config->res->ai_addr, config->res->ai_addrlen);
    if (errcode == -1) {
        die("could not connect to address");
    }
}

void send_message_from_stdin(int socket)
{
    char message[MAX_MESSAGE_SIZE];
    printf("Enter the message to send:\n");
    if (fgets(message, MAX_MESSAGE_SIZE, stdin) == NULL) {
        die("Could not read from stdin");
    }
    // Remove the potential newline
    message[strcspn(message, "\r\n")] = 0;
    printf("Entered: \"%s\"\n", message);

    int errcode = send(socket, message, strlen(message), 0);
    if (errcode == -1) {
        die("Could not send message");
    }
}


void receive_message_from_socket(int socket)
{
    char received_message[MAX_MESSAGE_SIZE];
    memset(received_message, 0, MAX_MESSAGE_SIZE);
    int errcode = recv(socket, received_message, MAX_MESSAGE_SIZE, 0);
    if (errcode == -1) {
        die("Could not receive message");
    }
    printf("Received: \"%s\"\n", received_message);
}


void free_socket_config(struct socket_config *config)
{
    freeaddrinfo(config->res);
    free(config);
}

void make_nonblocking_socket(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    int errcode = fcntl(fd, F_SETFL, flags);
    if (errcode == -1) {
        die("Could not make socket as non-blocking"); 
    }
}
