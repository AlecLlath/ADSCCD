#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_conf.h"

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in server_address;
    int connected_socket;
    ssize_t msg_size;
    uint32_t num_player = 4;
    uint32_t score[4];
    unsigned int i;

    score[0] = 67;
    score[1] = 23;
    score[2] = 113;
    score[3] = 35;

    /* creating a tcp-IPv4 socket*/
    connected_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connected_socket == -1) {
        fprintf(stderr, "Client error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* connect socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(TCP_PORT);
    if (!inet_aton(SERVER_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Client socket adress is not valid : %s \n",
                        SERVER_IPV4_ADDR);
        goto clean;
    }

    if (connect(connected_socket, (struct sockaddr *)&server_address,
                sizeof(server_address))) {
        fprintf(stderr, "Client error on connecting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    printf("My bowling team is connected to the umpire.\n");

    /* send the number of player */
    num_player = htonl(num_player);
    msg_size = send(connected_socket, &num_player, sizeof(num_player), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when sending number of player: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(num_player)) {
        fprintf(stderr, "Client only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(num_player));
        goto clean;
    }

    num_player = ntohl(num_player);
    printf("I sent the number of player to the empire.\n");
    
    /* send score of each player */
    for (i = 0; i < num_player; i++) {
        score[i] = htonl(score[i]);
        msg_size = send(connected_socket, &score[i], sizeof(score[i]), 0);
        if (msg_size == -1) {
            fprintf(stderr, "Client error when sending one client score: %d , %s .\n",
                            errno, strerror(errno));
            goto clean;
        }
    
        if (msg_size < sizeof(score[i])) {
            fprintf(stderr, "Client only sends %ld bytes instead of %ld .\n",
                            msg_size, sizeof(score[i]));
            goto clean;
        }
    
        score[i] = ntohl(score[i]);
    }
    printf("I sent the score of each player to the empire.\n");

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr, "Client fails to shutdown the connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Client error on closing connected socket : %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
    exit(EXIT_FAILURE);    
}
