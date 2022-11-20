#include <stdio.h>
#include <unistd.h>
#include "common.h"


int main(int argc, char *argv[])
{
    struct socket_config *config = configure_server("127.0.0.1", "1337");
    int socket = accept_connection(config);
    make_nonblocking_socket(socket);
    while (receive_message_from_socket(socket) == 1) {
        sleep(1);
        printf("Waiting %d second(s) to poll again\n", WAIT_PERIOD_SEC);
    }
    send_message_from_stdin(socket);
    return 0;
}

