#include <stdio.h>
#include <aio.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm-generic/errno.h>
#include "common.h"

#define SOCKET_AIOCB    0
#define STDIN_AIOCB     1
#define WAITING_PERIOD_ANIMATION 200 * 1000


int main(int argc, char *argv[])
{

    struct socket_config *config = configure_client("127.0.0.1", "1337");

    // Closes sockets
    signal(SIGINT, sigint_handler);

    connect_client(config);
    printf("Client connected\nWrite any message to send to the server. and Press Enter to send it at anytime\n");


    // Establish handler for the I/O completion signal
    struct sigaction act;
    struct sigaction act2;
    // Signals to masks: None
    sigemptyset(&act.sa_mask);
    sigemptyset(&act2.sa_mask);

    // Flags: Restart interrumpted system calls and use sa_sigaction to specify
    // signal handler
    act.sa_flags = SA_RESTART | SA_SIGINFO;
    act.sa_sigaction = handler_read;
    sigaction(SIGUSR1, &act, NULL);

    act2.sa_flags = SA_RESTART | SA_SIGINFO;
    act2.sa_sigaction = handler_write;
    sigaction(SIGUSR2, &act2, NULL);
    

    // Allocate two structures because we are listening to two file descriptors: // stdin and the connected socket
    struct aiocb *aiocb = calloc(2, sizeof(struct aiocb));
    memset(aiocb, 0, sizeof(*aiocb));

    aiocb[SOCKET_AIOCB].aio_fildes = config->socket;
    aiocb[STDIN_AIOCB].aio_fildes = STDIN_FILENO;

    struct async_read_writer socket_read_writer = {
        &aiocb[SOCKET_AIOCB],
        STDOUT_FILENO,
    };

    struct async_read_writer stdin_read_writer = {
        &aiocb[STDIN_AIOCB],
        config->socket,
    };

    
    for (int i = 0; i < 2; i++) {
        void *buf = malloc(MAX_MESSAGE_SIZE);
        if (buf == NULL) {
            die("malloc");
        }
        aiocb[i].aio_buf = buf;
        aiocb[i].aio_nbytes = MAX_MESSAGE_SIZE;
        aiocb[i].aio_reqprio = 0;
        aiocb[i].aio_offset = 0;
        aiocb[i].aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        aiocb[i].aio_sigevent.sigev_signo = SIGUSR1;
        if (i == SOCKET_AIOCB) {
            aiocb[i].aio_sigevent.sigev_value.sival_ptr = &socket_read_writer;
        } else {
            aiocb[i].aio_sigevent.sigev_value.sival_ptr = &stdin_read_writer;
        }

        if (aio_read(&aiocb[i]) == -1) {
            die("aio_read");
        }
    }

    /* bool got_input = false; */
    /* int state = 0; */
    int errcode;
    while (true) {
        /* if (!got_input) { */
        /*     state = waiting_animation(state); */
        /*     usleep(WAITING_PERIOD_ANIMATION); */
        /* } */
        for (int i = 0; i < 2; i++) {
            switch (errcode = aio_error(&aiocb[i])) {

                // request not completed yet
                case EINPROGRESS:
                    break;

                    // request was canceled
                case ECANCELED:
                    break;

                    // request completed successfully
                case 0:
                    if (aio_read(&aiocb[i]) == -1) {
                        die("aio_read");
                    }
                    continue;

                    // asynchronous I/O operation failed
                default:
                    die("aio_error");
            }
        }
    }

    return 0;
}
