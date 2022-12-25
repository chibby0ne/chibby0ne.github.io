#include <stdio.h>
#include <aio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <asm-generic/errno.h>
#include "common.h"

void die(char *message)
{
    perror(message);
    _exit(EXIT_FAILURE);
}

struct socket_config *configure_server(char *domain, char *service)
{
    struct socket_config *config = configure_client(domain, service);

    // Avoids the "Address already in use" error
    int yes = 1;
    setsockopt(config->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

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


int receive_message_from_socket(int socket)
{
    char received_message[MAX_MESSAGE_SIZE];
    memset(received_message, 0, MAX_MESSAGE_SIZE);
    int errcode = recv(socket, received_message, MAX_MESSAGE_SIZE, 0);
    if (errcode == -1) {
        die("Could not receive message");
    }
    if (errcode == 0) {
        printf("The connection closed on the other end. Exiting\n");
        _exit(0);
    }
    printf("Received: \"%s\"\n", received_message);
    return 0;
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

void sigint_handler(int signum)
{
    fprintf(stderr, "Received Ctrl-C. Exiting...\n");
    _exit(0);
}

void send_message_to_socket(char *message, int socket)
{
    // Remove the potential newline
    message[strcspn(message, "\r\n")] = 0;
    /* printf("Entered: \"%s\"\n", message); */

    int errcode = send(socket, message, strlen(message), 0);
    if (errcode == -1) {
        die("Could not send message");
    }
}

void receive_from_stdin_and_send_to_socket(int socket)
{
    char buf[BUFSIZ];
    memset(buf, 0, sizeof(buf));
    int nbytes = read(STDIN_FILENO, buf, sizeof(buf));
    if (nbytes == -1) {
        die("error reading from stdin");
    }
    if (nbytes == 0) {
        fprintf(stderr, "received EOF (Ctrl-D)");
        _exit(EXIT_SUCCESS);
    }
    send_message_to_socket(buf, socket);
}

void handler_write(int sig, siginfo_t *si, void *ucontext)
{
    if (si->si_code == SI_ASYNCIO) {
        fprintf(stdout, "Wrote asyncly to stdout or to socket");
    }
}

void memcpy_volatile(void *dest, volatile void *src, unsigned long size)
{
    char *cdest = (char *)dest;
    char *csrc = (char *)src;
    for (int i = 0; i < size; i++) {
        cdest[i] = csrc[i];
    }
}


void handler_read(int sig, siginfo_t *si, void *ucontext)
{
    if (si->si_code == SI_ASYNCIO) {
        struct async_read_writer *reader = (struct async_read_writer *) si->si_value.sival_ptr;

        char p[BUFSIZ];
        memcpy_volatile(p, reader->r->aio_buf, reader->r->aio_nbytes);
        p[strcspn(p, "\r\n")] = 0;
        if (reader->fildes_to_write == STDOUT_FILENO) {
            fprintf(stderr, "Received from socket: \"%s\"\n", p);
        } else {
            fprintf(stderr, "About to send to socket: \"%s\"\n", p);
            send_message_to_socket(p, reader->fildes_to_write);
        }

        /* struct aiocb *to_write = malloc(sizeof(struct aiocb)); */
        /* char *p = malloc(reader->r->aio_nbytes); */
        /* if (p == NULL) { */
        /*     die("malloc"); */
        /* } */
        /* memcpy_volatile(p, reader->r->aio_buf, reader->r->aio_nbytes); */
        /* to_write->aio_buf = p; */
        /* to_write->aio_nbytes = reader->r->aio_nbytes; */
        /* to_write->aio_fildes = reader->fildes_to_write; */
        /* to_write->aio_offset = 0; */
        /* to_write->aio_reqprio = 0; */
        /* to_write->aio_sigevent.sigev_notify = SIGEV_SIGNAL; */
        /* to_write->aio_sigevent.sigev_signo = SIGUSR2; */

        /* if (aio_write(to_write) == -1) { */
        /*     die("aio_write"); */
        /* } */
    }
}
