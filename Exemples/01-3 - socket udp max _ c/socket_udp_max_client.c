/* socket_udp_max_client.c */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
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
    socklen_t server_address_len = sizeof(server_address);
    int sock;
    ssize_t buf_size;
    ssize_t msg_size;
    void *buf;

    /* creating a udp-IPv4 socket*/
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        fprintf(stderr, "Client error on creating the sock : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }


    /* building server recipient address */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(UDP_PORT);
    if (!inet_aton(SERVER_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr,
                "Server socket address on client is not valid : %s \n",
                SERVER_IPV4_ADDR);
        goto clean;
    }

    /* send messages of increasing size */
    for (buf_size = STARTSIZE; buf_size <= MAXSIZE; buf_size = buf_size * 2) {
        /* alloc buf */
        buf = malloc(buf_size);
        if (!buf) {
            fprintf(stderr,
                    "Client unable to alloc a buffer of %zd bytes.\n", buf_size);
            goto clean;
        }

        /* send buffer */
        msg_size = sendto(sock, buf, buf_size, 0,
                          (const struct sockaddr *)&server_address,
                          sizeof(server_address));
        if (msg_size == -1) {
            fprintf(stderr, "Client error when sending client draw: %d , %s .\n",
                            errno, strerror(errno));
            free(buf);
            goto clean;
        }

        if (msg_size < buf_size) {
            fprintf(stderr, "Client only sends %d bytes instead of %d .\n",
                            msg_size, buf_size);
            free(buf);
            goto clean;
        }

        fprintf(stdout, "Client successfully sent %zd bytes.\n", msg_size);
        free(buf);
    }

clean:
    /* close the socket */
    if (close(sock)) {
        fprintf(stderr, "Client error on closing connected socket : %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
