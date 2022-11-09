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
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int listening_socket;
    int connected_socket;
    ssize_t msg_size;
    int fakebuf;
    uint32_t num_player;
    uint32_t top_score = 0;
    unsigned int top_player = 0;
    unsigned int i;

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
    if (listen(listening_socket, 1)) {
        fprintf(stderr, "Server unable to listen %d max client : %d , %s .\n",
                        1, errno, strerror(errno));
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
                        "large : %d accepted, instead of %ld .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    /* receive the number of player */
    msg_size = recv(connected_socket, &num_player, sizeof(num_player), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when receiving number of player: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(num_player)) {
        fprintf(stderr, "Server only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(num_player));
        goto clean;
    }

    num_player = ntohl(num_player);

    /* receives players scores */
    for (i = 0; i < num_player; i++) {
	uint32_t score;

        msg_size = recv(connected_socket, &score, sizeof(score), 0);
        if (msg_size == -1) {
            fprintf(stderr, "Server error when receiving one client score: %d , %s .\n",
                            errno, strerror(errno));
            goto clean;
        }
    
        if (msg_size < sizeof(score)) {
            fprintf(stderr, "Server only receives %ld bytes instead of %ld .\n",
                            msg_size, sizeof(score));
            goto clean;
        }
    
        score = ntohl(score);

	/* update scoreboard */
	if (score > top_score) {
		top_score = score;
		top_player = i;
	}
    }

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Server fails to shutdown the connected socket: "
                "%d , %s .\n", errno, strerror(errno));
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
    printf("%u players, best score %u from player %u .\n", num_player, top_score, top_player);
    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    exit(EXIT_FAILURE);    
}
