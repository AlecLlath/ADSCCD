/* socket_udp_draw_client.c */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "socket_udp_conf.h"

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in server_address;
    socklen_t server_address_len = sizeof(server_address);
    int sock;
    ssize_t msg_size;
    int fakebuf;
    /* random number variables */
    unsigned int seed;
    uint32_t client_draw;
    uint32_t server_draw;

    /* get my random number */
    seed = time(NULL);
    client_draw = (uint32_t)rand_r(&seed);

    /* creating a udp-IPv4 socket*/
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        fprintf(stderr, "Client error on creating the sock : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }


    /* send the client random number to the server */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(UDP_PORT);
    if (!inet_aton(SERVER_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Client socket adress is not valid : %s \n",
                        SERVER_IPV4_ADDR);
        goto clean;
    }

    client_draw = htonl(client_draw);
    msg_size = sendto(sock, &client_draw, sizeof(client_draw), 0,
                      (const struct sockaddr *)&server_address,
                      sizeof(server_address));
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
    msg_size = recvfrom(sock, &server_draw, sizeof(server_draw), 0,
                        (struct sockaddr *)&server_address, &server_address_len);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when receiving server draw: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (server_address_len > sizeof(server_address)) {
        fprintf(stderr,
                "Server address size is too large on client after recvfrom: %d, "
                "%d expected .\n", server_address_len, sizeof(server_address));
        goto clean;
    }

    if (msg_size < sizeof(server_draw)) {
        fprintf(stderr, "Client only receives %d bytes instead of %d .\n",
                        msg_size, sizeof(server_draw));
        goto clean;
    }

    server_draw = ntohl(server_draw);

    /* close the socket */
    if (close(sock)) {
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
    close(sock);
    exit(EXIT_FAILURE);    
}
