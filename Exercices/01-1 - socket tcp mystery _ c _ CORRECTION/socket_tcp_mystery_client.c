#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_tcp_conf.h"

#define CLIENT_ID 7

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in giver_address;
    struct sockaddr_in checker_address;
    int giver_socket;
    int checker_socket;
    ssize_t msg_size;
    /* random number variables */
    uint32_t client_id = CLIENT_ID;
    uint32_t client_key;

    /* tcp-IPv4 giver_socket*/
    giver_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (giver_socket == -1) {
        fprintf(stderr, "Client error on creating the giver socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* connect giver_socket */
    giver_address.sin_family = AF_INET;
    giver_address.sin_port = htons(GIVER_TCP_PORT);
    if (!inet_aton(GIVER_IPV4_ADDR, &giver_address.sin_addr)) {
        fprintf(stderr, "Giver socket address is not valid : %s \n",
                        GIVER_IPV4_ADDR);
        goto clean_giver;
    }

    if (connect(giver_socket, (struct sockaddr *)&giver_address,
                sizeof(giver_address))) {
        fprintf(stderr, "Client error on connecting giver_socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_giver;
    }

    /* send the client id to the giver server */
    client_id = htonl(client_id);
    msg_size = send(giver_socket, &client_id, sizeof(client_id), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when sending client id: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_giver;
    }

    if (msg_size < sizeof(client_id)) {
        fprintf(stderr, "Client only sends %d bytes instead of %d .\n",
                        msg_size, sizeof(client_id));
        goto clean_giver;
    }

    client_id = ntohl(client_id);
    printf("Client sent its id %u to the giver server.\n", client_id);
    

    /* receive the mystery key from giver */
    msg_size = recv(giver_socket, &client_key, sizeof(client_key), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when receiving mystery key: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_giver;
    }

    if (msg_size < sizeof(client_key)) {
        fprintf(stderr, "Client only receives %d bytes instead of %d .\n",
                        msg_size, sizeof(client_key));
        goto clean_giver;
    }

    client_key = ntohl(client_key);
    printf("Client received the mystery key %u .\n", client_key);

    /* tcp-IPv4 checker_socket*/
    checker_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (checker_socket == -1) {
        fprintf(stderr, "Client error on creating the checker socket : %d, %s.\n",
                        errno, strerror(errno));
        goto clean_giver;
    }

    /* connect checker_socket */
    checker_address.sin_family = AF_INET;
    checker_address.sin_port = htons(CHECKER_TCP_PORT);
    if (!inet_aton(CHECKER_IPV4_ADDR, &checker_address.sin_addr)) {
        fprintf(stderr, "Checker socket address is not valid : %s \n",
                        CHECKER_IPV4_ADDR);
        goto clean_checker;
    }

    if (connect(checker_socket, (struct sockaddr *)&checker_address,
                sizeof(checker_address))) {
        fprintf(stderr, "Client error on connecting checker_socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_checker;
    }

    /* send the client id to the checker server */
    client_id = htonl(client_id);
    msg_size = send(checker_socket, &client_id, sizeof(client_id), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when sending client id: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_checker;
    }

    if (msg_size < sizeof(client_id)) {
        fprintf(stderr, "Client only sends %d bytes instead of %d .\n",
                        msg_size, sizeof(client_id));
        goto clean_checker;
    }

    client_id = ntohl(client_id);
    printf("Client sent its id %u to the checker server.\n", client_id);

    /* send the client key to the checker server */
    client_key = htonl(client_key);
    msg_size = send(checker_socket, &client_key, sizeof(client_key), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when sending client key: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_checker;
    }

    if (msg_size < sizeof(client_key)) {
        fprintf(stderr, "Client only sends %d bytes instead of %d .\n",
                        msg_size, sizeof(client_key));
        goto clean_checker;
    }

    client_key = ntohl(client_key);
    printf("Client sent its key %u to the checker server.\n", client_key);
    

    printf("Client finished its job.\n");
    /* close the sockets */
    if (shutdown(checker_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Client fails to shutdown the checker socket: %d , %s .\n",
                errno, strerror(errno));
        goto clean_checker;
    }

    if (close(checker_socket)) {
        fprintf(stderr, "Client error on closing checker socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_giver;
    }

    if (shutdown(giver_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Client fails to shutdown the giver socket: %d , %s .\n",
                errno, strerror(errno));
        goto clean_giver;
    }

    if (close(giver_socket)) {
        fprintf(stderr, "Client error on closing giver socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

clean_checker:
    shutdown(checker_socket, SHUT_RDWR);
    close(checker_socket);
clean_giver:
    shutdown(giver_socket, SHUT_RDWR);
    close(giver_socket);
    exit(EXIT_FAILURE);    
}
