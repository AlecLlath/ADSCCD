#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "socket_conf.h"

int main(int argc, char ** argv)
{
    struct sockaddr_in server_address;
    int connected_socket;
    ssize_t msg_size;
    uint32_t password_length;
    char password[5];

    /* password */
    password[0] = 'I';
    password[1] = 'B';
    password[2] = 'G';
    password[3] = 'B';
    password[4] = 'I';
    password_length = 5;

    /* creating a tcp-IPv4 socket*/
    connected_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connected_socket == -1) {
        fprintf(stderr, "Password whisperer error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* connect socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(WHISPERER_CONNECTION_PORT);
    if (!inet_aton(WHISPERER_DESTINATION_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Client socket adress is not valid : %s \n",
                        WHISPERER_DESTINATION_IPV4_ADDR);
        goto clean;
    }

    if (connect(connected_socket, (struct sockaddr *)&server_address,
                sizeof(server_address))) {
        fprintf(stderr, "Client error on connecting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    printf("Password whispererd is connected.\n");

    /* send the number of byte in the password */
    password_length = htonl(password_length);
    msg_size = send(connected_socket, &password_length, sizeof(password_length), 0); 
    if (msg_size == -1) {
        fprintf(stderr, "Password whisperer error when sending number of bytes: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(password_length)) {
        fprintf(stderr, "Password whisperer as length only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(password_length));
        goto clean;
    }

    password_length = ntohl(password_length);
    printf("Password whisperer sent the password length.\n");

    /* whisper the password */
    msg_size = send(connected_socket, &password[0], password_length, 0); 
    if (msg_size == -1) {
        fprintf(stderr, "Password whisperer error when sending password : %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < password_length) {
        fprintf(stderr, "Password whisperer as password only sends %ld bytes instead of %u .\n",
                        msg_size, password_length);
        goto clean;
    }

    printf("Password whisperer whispered the password.\n");

    /* close the socket */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr, "Password whisperer fails to shutdown the connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Password whisperer error on closing connected socket : %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
    exit(EXIT_FAILURE);
}
