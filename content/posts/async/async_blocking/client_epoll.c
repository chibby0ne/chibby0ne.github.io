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

    }
    return 0;
}
