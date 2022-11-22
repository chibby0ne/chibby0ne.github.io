#include <stdio.h>
#include <sys/select.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "common.h"


int main(int argc, char *argv[])
{
    struct socket_config *config = configure_server("127.0.0.1", "1337");

    // Closes sockets
    signal(SIGINT, sigint_handler);

    int socket = accept_connection(config);
    printf("Server connected\nWrite any message to send to the client. and Press Enter to send it at anytime\n");

    int nfds = socket + 1;
    fd_set readfds;

    while (true) {
    }
    return 0;
}

