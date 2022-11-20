#ifndef COMMON_H
#define COMMON_H

#define MAX_MESSAGE_SIZE 1024
#define WAIT_PERIOD_SEC 1

struct socket_config {
    int socket;
    struct addrinfo *res;
};


void die(char *message);
struct socket_config *configure_server(char *domain, char *service);
int accept_connection(struct socket_config *config);
struct socket_config *configure_client(char *domain, char *service);
void send_message_from_stdin(int socket);
int receive_message_from_socket(int socket);
void connect_client(struct socket_config *config);
void free_socket_config(struct socket_config *config);
void make_nonblocking_socket(int fd);
void sigint_handler(int signum);
void send_message_to_socket(char *message, int socket);
void receive_from_stdin_and_send_to_socket(int socket);

#endif /* ifndef COMMON_H */
