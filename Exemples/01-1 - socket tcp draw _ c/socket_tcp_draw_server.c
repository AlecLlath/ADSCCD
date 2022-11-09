/* socket_tcp_draw_server.c */

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
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int listening_socket;
    int connected_socket;
    ssize_t msg_size;
    int fakebuf;
    /* random number variables */
    unsigned int seed;
    uint32_t client_draw;
    uint32_t server_draw;

    /* get my random number */
    seed = time(NULL);
    server_draw = (uint32_t)rand_r(&seed);

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Server error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(TCP_PORT);
    if (!inet_aton(SERVER_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Server socket adress is not valid : %s \n",
                        SERVER_IPV4_ADDR);
        goto clean_listening;
    }
    if (bind(listening_socket, (struct sockaddr *)&server_address,
             sizeof(server_address))) {
        fprintf(stderr, "Server error on binding : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* listen */
    if (listen(listening_socket, MAX_CLIENT)) {
        fprintf(stderr, "Server unable to listen %d max client : %d , %s .\n",
                        MAX_CLIENT, errno, strerror(errno));
        goto clean_listening;
    }
    
    /* accept one connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Server error on accepting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Server error on accepting a client address size too "
                        "large : %d accepted, instead of %d .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    /* receive the client random number */
    msg_size = recv(connected_socket, &client_draw, sizeof(client_draw), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when receiving client draw: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(client_draw)) {
        fprintf(stderr, "Server only receives %d bytes instead of %d .\n",
                        msg_size, sizeof(client_draw));
        goto clean;
    }

    client_draw = ntohl(client_draw);
    

    /* send the server random number to the client */
    server_draw = htonl(server_draw);
    msg_size = send(connected_socket, &server_draw, sizeof(server_draw), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when sending server draw: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(server_draw)) {
        fprintf(stderr, "Server only sends %d bytes instead of %d .\n",
                        msg_size, sizeof(server_draw));
        goto clean;
    }

    server_draw = ntohl(server_draw);

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_WR)) {
        fprintf(stderr,
                "Server fails to shutdown write mode of the connected socket: "
                "%d , %s .\n", errno, strerror(errno));
    }

    msg_size = recv(connected_socket, &fakebuf, sizeof(fakebuf), MSG_PEEK);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when testing shutdown: %d , %s .\n",
                errno, strerror(errno));
        goto clean;
    }

    if (msg_size != 0) {
        fprintf(stderr, "Server receives %d bytes after closing.\n", msg_size);
        goto clean;
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Server error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (close(listening_socket)) {
        fprintf(stderr, "Server error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* print result */
    fprintf(stdout, "Server draws %d .\n", server_draw);
    fprintf(stdout, "Server gets %d as draw from the client.\n", client_draw);
    if (server_draw > client_draw)
        fprintf(stdout, "As server, my draw is upper.\n");
    else if (server_draw == client_draw)
        fprintf(stdout, "As server, I conclude to a perfect draw.\n");
    else
        fprintf(stdout, "As server, my draw is lower.\n");

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    exit(EXIT_FAILURE);    
}
