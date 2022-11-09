#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
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
    uint32_t password_length;
    char *buf_password = NULL;
    char password[5];
    uint32_t treasure_value;
    uint32_t i;

    /* password */
    password[0] = 'I';
    password[1] = 'B';
    password[2] = 'G';
    password[3] = 'B';
    password[4] = 'I';

    /* treasure value */
    treasure_value = 777;

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Vault error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(VAULT_TCP_PORT);
    if (!inet_aton(VAULT_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Vault socket address is not valid : %s \n",
                        VAULT_IPV4_ADDR);
        goto clean_listening;
    }
    if (bind(listening_socket, (struct sockaddr *)&server_address,
             sizeof(server_address))) {
        fprintf(stderr, "Vault error on binding : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* listen */
    if (listen(listening_socket, 1)) {
        fprintf(stderr, "Vault unable to listen %d max client : %d , %s .\n",
                        1, errno, strerror(errno));
        goto clean_listening;
    }
    
    /* accept one connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Vault error on accepting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Vault error on accepting a client address size too "
                        "large : %d accepted, instead of %ld .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    /* receive the length of the password */
    msg_size = recv(connected_socket, &password_length, sizeof(password_length), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Vault error when receiving password length: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(password_length)) {
        fprintf(stderr, "Vault only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(password_length));
        goto clean;
    }

    password_length = ntohl(password_length);
    printf("Vault received a password length.\n");

    /* allocate password */
    buf_password = malloc(password_length);
    if (!buf_password) {
        fprintf(stderr, "Vault error when allocating buf_password.\n");
	goto clean;
    }

    /* receive the password */
    msg_size = recv(connected_socket, buf_password, password_length, 0);
    if (msg_size == -1) {
        fprintf(stderr, "Vault error when receiving password: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < password_length) {
        fprintf(stderr, "Vault only receives %ld bytes instead of %u .\n",
                        msg_size, password_length);
        goto clean;
    }

    printf("Vault received a password.\n");

    /* check password */
    if (password_length != 5) {
        fprintf(stderr, "Vault received a wrong password.\n");
	goto clean;
    }

    for (i = 0; i < password_length; i++) {
        if (buf_password[i] != password[i]) {
            fprintf(stderr, "Vault received a wrong password.\n");
	    goto clean;
	}
    }

    printf("Vault attests the received password is the good one.\n");

    /* send treasure value */
    treasure_value = htonl(treasure_value);
    msg_size = send(connected_socket, &treasure_value, sizeof(treasure_value), 0); 
    if (msg_size == -1) {
        fprintf(stderr, "Vault error when sending treasure_value: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(password_length)) {
        fprintf(stderr, "Vault as treasure value only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(password_length));
        goto clean;
    }

    treasure_value = ntohl(treasure_value);
    printf("Vault sent the treasure_value.\n");

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Vault fails to shutdown the connected socket: "
                "%d , %s .\n", errno, strerror(errno));
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Vault error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (close(listening_socket)) {
        fprintf(stderr, "Vault error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    free(buf_password);
    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    free(buf_password);
    exit(EXIT_FAILURE);    
}
