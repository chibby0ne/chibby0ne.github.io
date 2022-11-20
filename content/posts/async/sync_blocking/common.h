#ifndef COMMON_H
#define COMMON_H

#define MAX_MESSAGE_SIZE 1024

struct socket_config {
    int socket;
    struct addrinfo *res;
};


void die(char *message);
struct socket_config *configure_server(char *domain, char *service);
int accept_connection(struct socket_config *config);
struct socket_config *configure_client(char *domain, char *service);
void send_message_from_stdin(int socket);
void receive_message_from_socket(int socket);
void connect_client(struct socket_config *config);
void free_socket_config(struct socket_config *config);
void make_nonblocking_socket(int fd);

#endif /* ifndef COMMON_H */
