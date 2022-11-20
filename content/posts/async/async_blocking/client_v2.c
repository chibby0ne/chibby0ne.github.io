#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "common.h"


int main(int argc, char *argv[])
{
    struct socket_config *config = configure_client("127.0.0.1", "1337");

    // Closes sockets
    signal(SIGINT, sigint_handler);

    connect_client(config);
    printf("Client connected\nWrite any message to send to the server. and Press Enter to send it at anytime\n");

    int nfds = config->socket + 1;
    fd_set readfds;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(config->socket, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        int errcode = select(nfds, &readfds, NULL, NULL, NULL);
        if (errcode == -1) {
            die("error select'ing");
        }
        if (errcode == 0) {
            fprintf(stderr, "Timeout happened\n");
            _exit(0);
        }

        // Received data from stdin (read it and send it to server)
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            receive_from_stdin_and_send_to_socket(config->socket);
        }

        // Received data from socket (received it from client)
        if (FD_ISSET(config->socket, &readfds)) {
            receive_message_from_socket(config->socket);
        }

    }
    return 0;
}
