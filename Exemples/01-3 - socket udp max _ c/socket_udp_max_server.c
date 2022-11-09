/* socket_udp_max_server.c */

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

#define KILOBYTE (1<<10)
#define MEGABYTE (1<<20)
#define STARTSIZE KILOBYTE
#define MAXSIZE MEGABYTE

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int sock;
    ssize_t buf_size;
    ssize_t msg_size;
    void *buf;

    /* creating a udp-IPv4 socket*/
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        fprintf(stderr, "Server error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(UDP_PORT);
    if (!inet_aton(SERVER_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Server socket adress is not valid : %s \n",
                        SERVER_IPV4_ADDR);
        goto clean;
    }
    if (bind(sock, (struct sockaddr *)&server_address,
             sizeof(server_address))) {
        fprintf(stderr, "Server error on binding : %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    /* receive messages of increasing size */
    for (buf_size = STARTSIZE; buf_size <= MAXSIZE; buf_size = buf_size * 2) {
        /* alloc buf */
        buf = malloc(buf_size);
        if (!buf) {
            fprintf(stderr,
                    "Server unable to alloc a buffer of %zd bytes.\n", buf_size);
            goto clean;
        }

        /* receive buffer */
        msg_size = recvfrom(sock, buf, buf_size, 0,
                            (struct sockaddr *)&client_address,
                            &client_address_len);
        if (msg_size == -1) {
            fprintf(stderr,
                    "Server error when receiving client draw: %d , %s .\n",
                    errno, strerror(errno));
            free(buf);
            goto clean;
        }
    
        if (client_address_len > sizeof(client_address)) {
            fprintf(stderr,
                    "Client address size is too large on client after recvfrom: "
                    "%d, %d expected.\n",
                    client_address_len, sizeof(client_address));
            free(buf);
            goto clean;
        }
    
        if (msg_size < buf_size) {
            fprintf(stderr,
                    "Server only receives %d bytes instead of %d .\n",
                    msg_size, buf_size);
            free(buf);
            goto clean;
        }

        fprintf(stdout, "Server successfully received %zd bytes.\n", msg_size);
        free(buf);
    }

clean:
    /* close the socket */
    if (close(sock)) {
        fprintf(stderr, "Server error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);    
}
