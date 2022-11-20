.PHONY: all
all: server_v2 client_v2 clean compilation_database

.PHONY: server_v2
server_v2:
	gcc server_v2.c common.c -Wall -g -o server_v2

.PHONY: client_v2
client_v2:
	gcc client_v2.c common.c -Wall -g -o client_v2

.PHONY: clean
clean:
	rm -f client_v2 server_v2

compilation_database:
	bear -- make
