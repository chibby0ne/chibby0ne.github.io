.PHONY: all
all: server client server_v2 client_v2 clean compilation_database

.PHONY: server
server:
	gcc server.c -Wall -g -o server

.PHONY: client
client:
	gcc client.c -Wall -g -o client

.PHONY: server_v2
server_v2:
	gcc server_v2.c common.c -Wall -g -o server_v2

.PHONY: client_v2
client_v2:
	gcc client_v2.c common.c -Wall -g -o client_v2

.PHONY: clean
clean:
	rm -f client server client_v2 server_v2

compilation_database:
	bear -- make
