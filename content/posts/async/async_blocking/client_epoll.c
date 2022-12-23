#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "common.h"

#define MAX_EVENTS  10
#define TIMEOUT     10

int main(int argc, char *argv[])
{
    struct socket_config *config = configure_client("127.0.0.1", "1337");

    // Closes sockets
    signal(SIGINT, sigint_handler);

    connect_client(config);
    printf("Client connected\nWrite any message to send to the server. and Press Enter to send it at anytime\n");

    // Create epoll instance
    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        die("epoll_create");
    }

    // Add the file descriptors for the connected socket and the stdin to the interest list 
    // Only interested when these file descriptors are ready for read (or recv)
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = config->socket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, config->socket, &event) == -1) {
        die("epoll_ctl");
    }

    event.data.fd = STDIN_FILENO;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        die("epoll_ctl");
    }
    
    struct epoll_event events[MAX_EVENTS];
    while (true) {

        // the main thread will block here for up to TIMEOUT seconds or until an
        // event happens
        // This is the reason why this is considered async but blocking (notification)
        // To make it async and nonblocking all that's needed is to set the
        // TIMEOUT to 0
        int nfds_ready = epoll_wait(epollfd, events, MAX_EVENTS, TIMEOUT);
        if (nfds_ready == -1) {
            die("epoll_wait");
        }
        // Some file descriptors are ready for read
        if (nfds_ready > 0) {
            for (int i = 0; i < nfds_ready; i++) {
                if (events[i].data.fd == config->socket) {
                    receive_message_from_socket(config->socket);
                }

                if (events[i].data.fd == STDIN_FILENO) {
                    send_message_from_stdin(config->socket);
                }
            }
        }
    }
    return 0;
}
