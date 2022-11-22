#include <stdio.h>
#include <signal.h>
#include "common.h"

int main(int argc, char *argv[])
{
    struct socket_config *config = configure_server("127.0.0.1", "1337");

    // Closes sockets
    signal(SIGINT, sigint_handler);

    int socket = accept_connection(config);
    printf("Server connected\nWrite any message to send to the client. and Press Enter to send it at anytime\n");


    return 0;
}
