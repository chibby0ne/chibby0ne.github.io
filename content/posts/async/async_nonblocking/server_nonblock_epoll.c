#include <stdio.h>
#include <sys/select.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "common.h"

#define MAX_EVENTS 10


int main(int argc, char *argv[])
{
    struct socket_config *config = configure_server("127.0.0.1", "1337");

    // Closes sockets
    signal(SIGINT, sigint_handler);

    int socket = accept_connection(config);
    printf("Server connected\nWrite any message to send to the client. and Press Enter to send it at anytime\n");

    // Create epoll instance
    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        die("epoll_create");
    }

    // Add the file descriptors for the connected socket and the stdin to the interest list 
    // Only interested when these file descriptors are ready for read (or recv)
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket, &event) == -1) {
        die("epoll_ctl");
    }

    event.data.fd = STDIN_FILENO;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        die("epoll_ctl");
    }
    
    struct epoll_event events[MAX_EVENTS];
    while (true) {

        int nfds_ready = epoll_wait(epollfd, events, MAX_EVENTS, 0);
        if (nfds_ready == -1) {
            die("epoll_wait");
        }
        // Some file descriptors are ready for read
        if (nfds_ready > 0) {
            for (int i = 0; i < nfds_ready; i++) {
                if (events[i].data.fd == socket) {
                    receive_message_from_socket(socket);
                }

                if (events[i].data.fd == STDIN_FILENO) {
                    send_message_from_stdin(socket);
                }
            }
        }
    }
    return 0;
}

