#include <stdio.h>
#include "common.h"


int main(int argc, char *argv[])
{
    struct socket_config *config = configure_server("127.0.0.1", "1337");
    int socket = accept_connection(config);
    receive_message_from_socket(socket);
    send_message_from_stdin(socket);
    return 0;
}

