#include <stdio.h>
#include <aio.h>
#include "common.h"
#include <signal.h>

int main(int argc, char *argv[])
{
    struct socket_config *config = configure_client("127.0.0.1", "1337");

    // Closes sockets
    signal(SIGINT, sigint_handler);

    connect_client(config);
    printf("Client connected\nWrite any message to send to the server. and Press Enter to send it at anytime\n");



    return 0;
}
