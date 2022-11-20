#include <stdio.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char *argv[])
{
    struct socket_config *config = configure_client("127.0.0.1", "1337");
    connect_client(config);
    send_message_from_stdin(config->socket);
    // Without this make_nonblocking_socket the program blocks on the receive
    // function. Try compiling/running the program without it and with it to
    // compare the behaviour!
    make_nonblocking_socket(config->socket);
    while (receive_message_from_socket(config->socket) == 1) {
        sleep(1);
        printf("Waiting %d second(s) to poll again\n", WAIT_PERIOD_SEC);
    }
    return 0;
}
