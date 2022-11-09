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
    uint32_t num_ticket;
    uint32_t one_ticket_price = 11;
    uint32_t global_price;
    uint32_t price_back;
    uint32_t booking_number = 768;

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

    printf("The broadway shop is opened and waits for its client.\n");
    
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
    printf("A client is connected.\n");

    /* receive num_ticket */
    msg_size = recv(connected_socket, &num_ticket, sizeof(num_ticket), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when receiving number of ticket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(num_ticket)) {
        fprintf(stderr, "Server only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(num_ticket));
        goto clean;
    }

    num_ticket = ntohl(num_ticket);
    printf("The client asks for %u tickets.\n", num_ticket);
    
    /* send the price */
    global_price = one_ticket_price * num_ticket;
    global_price = htonl(global_price);
    msg_size = send(connected_socket, &global_price, sizeof(global_price), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when sending global price: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(global_price)) {
        fprintf(stderr, "Server only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(global_price));
        goto clean;
    }

    global_price = ntohl(global_price);
    printf("I sent the global price to the client.\n");

    /* receive price_back */
    msg_size = recv(connected_socket, &price_back, sizeof(price_back), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when receiving price_back draw: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(price_back)) {
        fprintf(stderr, "Server only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(price_back));
        goto clean;
    }

    price_back = ntohl(price_back);
    printf("I received %u as payment from the client.\n", price_back);
    if (price_back != global_price) {
        fprintf(stderr, "Error, the payment as the wrong value.\n");
	goto clean;
    }

    /* send the booking number */
    booking_number = htonl(booking_number);
    msg_size = send(connected_socket, &booking_number, sizeof(booking_number), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when sending global price: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(booking_number)) {
        fprintf(stderr, "Server only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(booking_number));
        goto clean;
    }

    booking_number = ntohl(booking_number);
    printf("I sent the booking number to the client.\n");

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Server fails to shutdown write mode of the connected socket: "
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

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    exit(EXIT_FAILURE);    
}
