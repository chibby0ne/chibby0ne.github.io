#include <stdio.h>
#include "common.h"

int main(int argc, char *argv[])
{
    struct socket_config *config = configure_client("127.0.0.1", "1337");
    connect_client(config);
    send_message_from_stdin(config->socket);
    receive_message_from_socket(config->socket);
    return 0;
}

