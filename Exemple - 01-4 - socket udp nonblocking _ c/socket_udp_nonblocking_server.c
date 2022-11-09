/* socket_udp_nonblocking_server.c */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_udp_conf.h"

#define WAIT_SECOND 1

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int sock;
    ssize_t msg_size;
    char msg_value;

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

    /* receive message on a nonblocking cycle */
    while (1) {
        msg_size = recvfrom(sock, &msg_value, sizeof(msg_value), MSG_DONTWAIT,
                            (struct sockaddr *)&client_address,
                            &client_address_len);
        if (msg_size == -1) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                fprintf(stderr,
                        "Server error when receiving client draw: %d , %s .\n",
                        errno, strerror(errno));
                goto clean;
            }

            fprintf(stdout,
                    "Server is waiting %d s to receive a message.\n",
                    WAIT_SECOND);
            sleep(WAIT_SECOND);
            continue;
        }
    
        if (client_address_len > sizeof(client_address)) {
            fprintf(stderr,
                    "Client address size is too large on client after recvfrom: "
                    "%d, %d expected.\n",
                    client_address_len, sizeof(client_address));
            goto clean;
        }
    
        if (msg_size < sizeof(msg_value)) {
            fprintf(stderr,
                    "Server only receives %d bytes instead of %d .\n",
                    msg_size, sizeof(msg_value));
            goto clean;
        }

        break;
    }

    fprintf(stdout,
            "Server successfully received the message value: %d.\n", msg_value);

clean:
    /* close the socket */
    if (close(sock)) {
        fprintf(stderr, "Server error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);    
}
