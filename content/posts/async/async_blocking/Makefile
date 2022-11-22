.PHONY: all
all: clean server client

.PHONY: server
server:
	gcc server.c common.c -Wall -g -o server
	gcc server_epoll.c common.c -Wall -g -o server_epoll

.PHONY: client
client:
	gcc client.c common.c -Wall -g -o client
	gcc client_epoll.c common.c -Wall -g -o client_epoll

.PHONY: clean
clean:
	rm -f client server client_epoll server_epoll

compilation_database:
	bear -- make
