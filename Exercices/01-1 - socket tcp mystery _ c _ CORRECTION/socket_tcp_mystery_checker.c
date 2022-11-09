#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_tcp_conf.h"

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in checker_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int listening_socket;
    int connected_socket;
    ssize_t msg_size;
    /* random number variables */
    uint32_t client_id;
    uint32_t recv_client_key;
    uint32_t generated_client_key;

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Checker error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    checker_address.sin_family = AF_INET;
    checker_address.sin_port = htons(CHECKER_TCP_PORT);
    if (!inet_aton(CHECKER_IPV4_ADDR, &checker_address.sin_addr)) {
        fprintf(stderr, "Checker socket adress is not valid : %s \n",
                        CHECKER_IPV4_ADDR);
        goto clean_listening;
    }

    if (bind(listening_socket, (struct sockaddr *)&checker_address,
             sizeof(checker_address))) {
        fprintf(stderr, "Checker error on binding : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* listen */
    if (listen(listening_socket, MAX_CLIENT)) {
        fprintf(stderr, "Checker unable to listen %d max client : %d , %s .\n",
                        MAX_CLIENT, errno, strerror(errno));
        goto clean_listening;
    }

    printf("Checker is ready to check the client.\n");
    
    /* accept one connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Checker error on accepting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Checker error on accepting a client address size too "
                        "large : %d accepted, instead of %d .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    /* receive the client id */
    msg_size = recv(connected_socket, &client_id, sizeof(client_id), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Checker error when receiving client id: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(client_id)) {
        fprintf(stderr, "Checker only receives %d bytes instead of %d .\n",
                        msg_size, sizeof(client_id));
        goto clean;
    }

    client_id = ntohl(client_id);
    printf("Checker received the client id %u .\n", client_id);
    generated_client_key = key_generator(client_id);
    

    /* receive the client key */
    msg_size = recv(connected_socket, &recv_client_key, sizeof(recv_client_key),
                    0);
    if (msg_size == -1) {
        fprintf(stderr, "Checker error when receiving client key: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(recv_client_key)) {
        fprintf(stderr, "Checker only receives %d bytes instead of %d .\n",
                        msg_size, sizeof(recv_client_key));
        goto clean;
    }

    recv_client_key = ntohl(recv_client_key);
    printf("Checker received the client key %u .\n", recv_client_key);
    if (recv_client_key == generated_client_key)
        printf("Congrats from Checker : client %u provides good mystery key %u "
               " .\n", client_id, recv_client_key);
    else
        printf("Fallacy ! Checker confirms client %u provides the wrong key "
                "%u .\n", client_id, recv_client_key);

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Checker fails to shutdown the connected socket: %d , %s .\n",
                errno, strerror(errno));
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Checker error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (close(listening_socket)) {
        fprintf(stderr, "Checker error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Checker ends.\n");
    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    exit(EXIT_FAILURE);    
}
