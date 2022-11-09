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
    struct sockaddr_in vault_address;
    int vault_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int listening_socket;
    int connected_socket;
    ssize_t msg_size;
    uint32_t password_length;
    char *buf_password = NULL;
    uint32_t treasure_value;

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Player error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(WHISPERER_CONNECTION_PORT);
    if (!inet_aton(WHISPERER_DESTINATION_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Player socket address is not valid : %s \n",
                        WHISPERER_DESTINATION_IPV4_ADDR);
        goto clean_listening;
    }

    if (bind(listening_socket, (struct sockaddr *)&server_address,
             sizeof(server_address))) {
        fprintf(stderr, "Player error on binding : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* listen */
    if (listen(listening_socket, 1)) {
        fprintf(stderr, "Player unable to listen %d max client : %d , %s .\n",
                        1, errno, strerror(errno));
        goto clean_listening;
    }
    
    /* accept one connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Player error on accepting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Player error on accepting a client address size too "
                        "large : %d accepted, instead of %ld .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    /* receive the length of the password */
    msg_size = recv(connected_socket, &password_length, sizeof(password_length), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Player error when receiving password length: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(password_length)) {
        fprintf(stderr, "Player only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(password_length));
        goto clean;
    }

    password_length = ntohl(password_length);
    printf("Player received the password length %u .\n", password_length);

    /* allocate password */
    /* we add a final 0 to ease to print */
    buf_password = calloc(password_length + 1, sizeof(*buf_password));
    if (!buf_password) {
        fprintf(stderr, "Player error when allocating buf_password.\n");
	goto clean;
    }

    /* receive the password */
    msg_size = recv(connected_socket, buf_password, password_length, 0);
    if (msg_size == -1) {
        fprintf(stderr, "Player error when receiving password: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < password_length) {
        fprintf(stderr, "Player only receives %ld bytes instead of %u .\n",
                        msg_size, password_length);
        goto clean;
    }

    printf("Player received the password : %s .\n", buf_password);

    /* connect to vault */
    /* creating a tcp-IPv4 socket*/
    vault_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (vault_socket == -1) {
        fprintf(stderr, "Player error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* connect socket */
    vault_address.sin_family = AF_INET;
    vault_address.sin_port = htons(VAULT_TCP_PORT);
    if (!inet_aton(VAULT_IPV4_ADDR, &vault_address.sin_addr)) {
        fprintf(stderr, "Vault socket adress is not valid : %s \n",
                        VAULT_IPV4_ADDR);
        goto clean_vault;
    }

    if (connect(vault_socket, (struct sockaddr *)&vault_address,
                sizeof(vault_address))) {
        fprintf(stderr, "Palyer error on connecting vault socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_vault;
    }

    printf("Player is connected to vault.\n");

    /* send the number of byte in the password */
    password_length = htonl(password_length);
    msg_size = send(vault_socket, &password_length, sizeof(password_length), 0); 
    if (msg_size == -1) {
        fprintf(stderr, "Player error when sending number of bytes: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_vault;
    }

    if (msg_size < sizeof(password_length)) {
        fprintf(stderr, "Player as length only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(password_length));
        goto clean_vault;
    }

    password_length = ntohl(password_length);
    printf("Player sent the password length.\n");

    /* whisper the password */
    msg_size = send(vault_socket, buf_password, password_length, 0); 
    if (msg_size == -1) {
        fprintf(stderr, "Player error when sending password : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_vault;
    }

    if (msg_size < password_length) {
        fprintf(stderr, "Player as password only sends %ld bytes instead of %u .\n",
                        msg_size, password_length);
        goto clean_vault;
    }

    printf("Player whispered the password to vault.\n");

    /* receive the treasure value */
    msg_size = recv(vault_socket, &treasure_value, sizeof(treasure_value), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Player error when receiving treasure_value: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_vault;
    }

    if (msg_size < sizeof(treasure_value)) {
        fprintf(stderr, "Player only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(treasure_value));
        goto clean_vault;
    }

    treasure_value = ntohl(treasure_value);
    printf("Player received the treasure_value %u .\n", treasure_value);

    /* close sockets */
    if (shutdown(vault_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Player fails to shutdown the vault socket: "
                "%d , %s .\n", errno, strerror(errno));
    }

    if (close(vault_socket)) {
        fprintf(stderr, "Player error on closing vault socket: %d , %s .\n",
                        errno, strerror(errno));
	goto clean;
    }

    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Player fails to shutdown the connected socket: "
                "%d , %s .\n", errno, strerror(errno));
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Player error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (close(listening_socket)) {
        fprintf(stderr, "Player error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
	goto free_buf;
    }

    free(buf_password);
    exit(EXIT_SUCCESS);

clean_vault:
    close(vault_socket);
clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
free_buf:
    free(buf_password);
    exit(EXIT_FAILURE);
}



