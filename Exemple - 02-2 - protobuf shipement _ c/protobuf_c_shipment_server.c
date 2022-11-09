/* protobuf_c_shipment_server.c */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_tcp_conf.h"
#include "shipment.pb-c.h"

int main(int argc, char **argv)
{
    /* socket variables */
    struct addrinfo address_hints = {0, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL,
                                     NULL};
    struct addrinfo *server_address_list = NULL;
    struct sockaddr_in client_address[NB_CLIENT];
    socklen_t client_address_len = sizeof(client_address[0]);
    int listening_socket;
    int connected_socket;
    uint8_t *buf = NULL;
    int rc;
    int i;

    /* shipment result */
    uint32_t conference_kg = 0;
    uint32_t williams_kg = 0;
    uint32_t golden_kg = 0;
    uint32_t gala_kg = 0;

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Server error on creating the socket: %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* get address */
    rc = getaddrinfo(SERVER_NAME, TCP_PORT, &address_hints,
                     &server_address_list);
    if (rc) {
        fprintf(stderr, "Server unable to get own adress : %d , %s .\n",
                        rc, gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    if (bind(listening_socket, server_address_list->ai_addr,
             server_address_list->ai_addrlen)) {
        fprintf(stderr, "Server error on binding: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    freeaddrinfo(server_address_list); server_address_list = NULL;

    /* listen */
    if (listen(listening_socket, NB_CLIENT)) {
        fprintf(stderr, "Server unable to listen %d clients: %d , %s .\n",
                        NB_CLIENT, errno, strerror(errno));
        goto clean_listening;
    }
    
    /* get shipment of each client */
    for (i = 0; i < NB_CLIENT; i++) {
        uint32_t msg_size = 0;
        Shipment *shipment;
        size_t nb_bytes;
        uint32_t nb_kg = 0;
        size_t j = 0;
        Box **boxes;

        /* get one client connection */
        connected_socket = accept(listening_socket,
                                  (struct sockaddr *)&client_address,
                                  &client_address_len);
        if (connected_socket == -1) {
            fprintf(stderr, "Server error on accepting socket: %d , %s .\n",
                            errno, strerror(errno));
            goto clean_listening;
        }
    
        if (client_address_len > sizeof(client_address)) {
            fprintf(stderr, "Server error on accepting a client address size too "
                            "large: %d accepted, instead of %d .\n",
                            client_address_len, sizeof(client_address));
            goto clean;
        }

        /* get message size*/
        nb_bytes = recv(connected_socket, &msg_size, sizeof(msg_size), 0);
        if (nb_bytes == -1) {
            fprintf(stderr, "Server error when receiving size: %d , %s .\n",
                            errno, strerror(errno));
            goto clean;
        }

        /* check partial read */
        if (nb_bytes != sizeof(msg_size)) {
            fprintf(stderr, "Server got a partial read on getting size: %d "
                            "instead of %d .\n", nb_bytes, sizeof(msg_size));
            goto clean;
        }

        msg_size = ntohl(msg_size);

        /* alloc message buf */
        buf = malloc(msg_size);
        if (!buf) {
            fprintf(stderr, "Server error on allocating message buf of "
                            "size: %d .\n", msg_size);
            goto clean;
        }

        /* read shipment message */
        nb_bytes = recv(connected_socket, buf, msg_size, 0);
        if (nb_bytes == -1) {
            fprintf(stderr, "Server error when receiving message: %d , %s .\n",
                            errno, strerror(errno));
            goto clean;
        }

        /* check partial read */
        if (nb_bytes != msg_size) {
            fprintf(stderr, "Server got a partial read on message: %d "
                            "instead of %d .\n", nb_bytes, msg_size);
            goto clean;
        }

        /* store shipment entry */
        shipment = shipment__unpack(NULL, msg_size, buf);
        if (!shipment) {
            fprintf(stderr, "Server error when unpacking a message.\n");
            goto clean;
        }

        /* free buffer */
        free(buf); buf = NULL;

        /* compute nb kg with repeated field */
        boxes = shipment->box;
        for (j = 0; j < shipment->n_box; j++) {
            fprintf(stdout, "Server: box %d, %d kg .\n", j, boxes[j]->weight_in_kg);
            nb_kg += boxes[j]->weight_in_kg;
        }

        /* add kg to variety with oneof field */
        switch (shipment->box_type_case) {
        case SHIPMENT__BOX_TYPE_APPLE:
            switch (shipment->apple) {
            case APPLE_VARIETY__GOLDEN:
                golden_kg += nb_kg;
                fprintf(stdout, "Server receives %d boxes of GOLDEN, %d kg.\n",
                                shipment->n_box, nb_kg);
                break;
            case APPLE_VARIETY__GALA:
                gala_kg += nb_kg;
                fprintf(stdout, "Server receives %d boxes of GALA, %d kg.\n",
                                shipment->n_box, nb_kg);
                break;
            default:
                fprintf(stderr, "Server must receive GOLDEN or GALA apples.\n");
            }
            break;
        case SHIPMENT__BOX_TYPE_PEAR:
            switch (shipment->pear) {
            case PEAR_VARIETY__WILLIAMS:
                williams_kg += nb_kg;
                fprintf(stdout, "Server receives %d boxes of WILLIAMS, %d kg.\n",
                                shipment->n_box, nb_kg);
                break;
            case PEAR_VARIETY__CONFERENCE:
                conference_kg += nb_kg;
                fprintf(stdout, "Server receives %d boxes of CONFERENCE, %d kg.\n",
                                shipment->n_box, nb_kg);
                break;
            default:
                fprintf(stderr, "Server must receive WILLIAMS or CONFERENCE pears.\n");
            }
            break;
        default:
            fprintf(stderr, "Server must receive an apple or pear variety.\n");
        }
        
        /* free message */
        shipment__free_unpacked(shipment, NULL);

        /* close connected socket */
        if(close(connected_socket)) {
            fprintf(stderr, "Server error on closing connected socket: %d , %s .\n",
                            errno, strerror(errno));
            goto clean_listening;
        }
    }

    /* close listening socket */
    if(close(listening_socket)) {
        fprintf(stderr, "Server error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* print result */
    fprintf(stdout, "Server final stock :\n");
    fprintf(stdout, "nb kg of golden: %d .\n", golden_kg);
    fprintf(stdout, "nb kg of gala: %d .\n", gala_kg);
    fprintf(stdout, "nb kg of williams: %d .\n", williams_kg);
    fprintf(stdout, "nb kg of conference: %d .\n", conference_kg);
    
    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    freeaddrinfo(server_address_list);
    free(buf);
    exit(EXIT_FAILURE);    
}
