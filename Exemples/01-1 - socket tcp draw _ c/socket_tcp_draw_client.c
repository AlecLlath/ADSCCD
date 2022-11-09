/* socket_tcp_draw_client.c */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "socket_tcp_conf.h"

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in server_address;
    int connected_socket;
    ssize_t msg_size;
    int fakebuf;
    /* random number variables */
    unsigned int seed;
    uint32_t client_draw;
    uint32_t server_draw;

    /* get my random number */
    seed = time(NULL);
    client_draw = (uint32_t)rand_r(&seed);

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

    /* send the client random number to the server */
    client_draw = htonl(client_draw);
    msg_size = send(connected_socket, &client_draw, sizeof(client_draw), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when sending client draw: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(client_draw)) {
        fprintf(stderr, "Client only sends %d bytes instead of %d .\n",
                        msg_size, sizeof(client_draw));
        goto clean;
    }

    client_draw = ntohl(client_draw);
    

    /* receive the server random number from the server */
    msg_size = recv(connected_socket, &server_draw, sizeof(server_draw), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when receiving server draw: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(server_draw)) {
        fprintf(stderr, "Client only receives %d bytes instead of %d .\n",
                        msg_size, sizeof(server_draw));
        goto clean;
    }

    server_draw = ntohl(server_draw);

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_WR)) {
        fprintf(stderr, "Client fails to shutdown the connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    msg_size = recv(connected_socket, &fakebuf, sizeof(fakebuf), MSG_PEEK);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when testing shutdown: %d , %s .\n",
                errno, strerror(errno));
        goto clean;
    }

    if (msg_size != 0) {
        fprintf(stderr, "Client receives %d bytes after closing.\n", msg_size);
        goto clean;
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Client error on closing connected socket : %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* print result */
    fprintf(stdout, "Client draws %d .\n", client_draw);
    fprintf(stdout, "Client gets %d as draw from the server.\n", server_draw);
    if (client_draw > server_draw)
        fprintf(stdout, "As client, my draw is upper.\n");
    else if (server_draw == client_draw)
        fprintf(stdout, "As client, I conclude to a perfect draw.\n");
    else
        fprintf(stdout, "As client, my draw is lower.\n");

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
    exit(EXIT_FAILURE);    
}
