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
    struct sockaddr_in giver_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int listening_socket;
    int connected_socket;
    ssize_t msg_size;
    /* random number variables */
    uint32_t client_id;
    uint32_t client_key;

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Giver error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    giver_address.sin_family = AF_INET;
    giver_address.sin_port = htons(GIVER_TCP_PORT);
    if (!inet_aton(GIVER_IPV4_ADDR, &giver_address.sin_addr)) {
        fprintf(stderr, "Giver socket adress is not valid : %s \n",
                        GIVER_IPV4_ADDR);
        goto clean_listening;
    }

    if (bind(listening_socket, (struct sockaddr *)&giver_address,
             sizeof(giver_address))) {
        fprintf(stderr, "Giver error on binding : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* listen */
    if (listen(listening_socket, MAX_CLIENT)) {
        fprintf(stderr, "Giver unable to listen %d max client : %d , %s .\n",
                        MAX_CLIENT, errno, strerror(errno));
        goto clean_listening;
    }

    printf("Giver is ready to answer the client.\n");
    
    /* accept one connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Giver error on accepting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Giver error on accepting a client address size too "
                        "large : %d accepted, instead of %d .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    /* receive the client id */
    msg_size = recv(connected_socket, &client_id, sizeof(client_id), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Giver error when receiving client id: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(client_id)) {
        fprintf(stderr, "Giver only receives %d bytes instead of %d .\n",
                        msg_size, sizeof(client_id));
        goto clean;
    }

    client_id = ntohl(client_id);
    printf("Giver received the client id %u .\n", client_id);
    client_key = key_generator(client_id);
    

    /* send the mystery key to the client */
    client_key = htonl(client_key);
    msg_size = send(connected_socket, &client_key, sizeof(client_key), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Giver error when sending mystery key: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(client_key)) {
        fprintf(stderr, "Giver only sends %d bytes instead of %d .\n",
                        msg_size, sizeof(client_key));
        goto clean;
    }

    printf("Giver sent its mystery key to the client.\n");

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Giver fails to shutdown the connected socket: %d , %s .\n",
                errno, strerror(errno));
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Giver error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (close(listening_socket)) {
        fprintf(stderr, "Giver error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Giver ends.\n");
    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    exit(EXIT_FAILURE);    
}
