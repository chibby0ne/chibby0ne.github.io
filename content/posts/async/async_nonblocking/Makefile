.PHONY: all
all: clean server client

.PHONY: server
server:
	gcc server.c common.c -Wall -g -o server
	gcc server_nonblock_epoll.c common.c -Wall -g -o server_nonblock_epoll
	gcc server_kernel_aio.c common.c -Wall -g -o server_kernel_aio
	gcc server_io_uring.c common.c -Wall -g -o server_io_uring

.PHONY: client
client:
	gcc client.c common.c -Wall -g -o client
	gcc client_nonblock_epoll.c common.c -Wall -g -o client_nonblock_epoll
	gcc client_kernel_aio.c common.c -Wall -g -o client_kernel_aio
	gcc client_io_uring.c common.c -Wall -g -o client_io_uring

.PHONY: clean
clean:
	rm -f client server client_nonblock_epoll server_nonblock_epoll client_kernel_aio server_kernel_aio client_io_uring server_io_uring

compilation_database:
	bear -- make
