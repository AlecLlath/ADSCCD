/* protobuf_c_shipment_client.c */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "socket_tcp_conf.h"
#include "shipment.pb-c.h"

#define MAX_NB_BOX 10
#define MAX_KG_PER_BOX 50

int main(int argc, char **argv)
{
    /* socket variables */
    struct addrinfo address_hints = {0, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL,
                                     NULL};
    struct addrinfo *server_address_list = NULL;
    int connected_socket;
    Box **boxes = NULL;
    unsigned int seed;
    Shipment shipment;
    uint32_t msg_size;
    size_t nb_bytes;
    size_t nb_box;
    uint8_t *buf;
    uint32_t nb_kg = 0;
    uint32_t i;
    int rc;

    /* creating a tcp-IPv4 socket*/
    connected_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connected_socket == -1) {
        fprintf(stderr, "Client error on creating the socket: %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* get address */
    rc = getaddrinfo(SERVER_NAME, TCP_PORT, &address_hints,
                     &server_address_list);
    if (rc) {
        fprintf(stderr, "Client unable to get server adress : %d , %s .\n",
                        rc, gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    /* connect socket */
    if (connect(connected_socket, server_address_list->ai_addr,
                server_address_list->ai_addrlen)) {
        fprintf(stderr, "Client error on connect: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    freeaddrinfo(server_address_list); server_address_list = NULL;

    /******************/
    /* BUILD SHIPMENT */
    /******************/
    shipment__init(&shipment);

    /* how many boxes */
    seed = time(NULL);
    nb_box = rand_r(&seed) % MAX_NB_BOX;
    boxes = malloc(nb_box * sizeof(*shipment.box));
    if (!boxes) {
        fprintf(stderr, "Client unable to alloc boxes.\n");
        goto clean;
    }

    shipment.n_box = nb_box;
    shipment.box = boxes;

    /* init all boxes */
    for (i = 0; i < nb_box; i++) {
        boxes[i] = malloc(sizeof(*boxes[i]));
        if (!boxes[i]) {
            fprintf(stderr, "Client unable to alloc box %d .\n", i);
            goto clean;
        }

        box__init(boxes[i]);
        boxes[i]->barcode = i;
        boxes[i]->weight_in_kg = rand_r(&seed) % MAX_KG_PER_BOX;
        fprintf(stdout, "Client : box %d, %d kg .\n", i, boxes[i]->weight_in_kg);
        nb_kg += boxes[i]->weight_in_kg;
    }

    /* apple or pear variety */
    if (rand_r(&seed) % 2) {
        shipment.box_type_case = SHIPMENT__BOX_TYPE_APPLE;
        if (rand_r(&seed) % 2) {
            shipment.apple = APPLE_VARIETY__GOLDEN;
            fprintf(stdout, "Client sends %d boxes of GOLDEN, %d kg .\n",
                            nb_box, nb_kg);
        } else {
            shipment.apple = APPLE_VARIETY__GALA;
            fprintf(stdout, "Client sends %d boxes of GALA, %d kg .\n",
                            nb_box, nb_kg);
        }
    } else {
        shipment.box_type_case = SHIPMENT__BOX_TYPE_PEAR;
        if (rand_r(&seed) % 2) {
            shipment.pear = PEAR_VARIETY__WILLIAMS;
            fprintf(stdout, "Client sends %d boxes of WILLIAMS, %d kg .\n",
                            nb_box, nb_kg);
        } else {
            shipment.pear = PEAR_VARIETY__CONFERENCE;
            fprintf(stdout, "Client sends %d boxes of CONFERENCE, %d kg .\n",
                            nb_box, nb_kg);
        }
    }

    /************/
    /* SEND MSG */
    /************/
    /* alloc buf */
    msg_size = shipment__get_packed_size(&shipment);
    buf = malloc(msg_size);
    if (!buf) {
        fprintf(stderr, "Client unable to alloc buf.\n");
        goto clean;
    }

    /* fill buf */
    nb_bytes = shipment__pack(&shipment, buf);
    if (nb_bytes != msg_size) {
        fprintf(stderr, "Client error on packing shipment : %d instead of %d "
                         ".\n", nb_bytes, msg_size);
        goto clean;
    }

    /* send size */
    msg_size = htonl(msg_size);
    nb_bytes = send(connected_socket, &msg_size, sizeof(msg_size), 0);
    if (nb_bytes == -1) {
        fprintf(stderr, "Client error on sending msg size: %d, %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    msg_size = ntohl(msg_size);

    if (nb_bytes != sizeof(msg_size)) {
        fprintf(stderr, "Client partial write on sending size : %d instead "
                        "of %d .\n", nb_bytes, sizeof(msg_size));
    }

    /* send shipment */
    nb_bytes = send(connected_socket, buf, msg_size, 0);
    if (nb_bytes == -1) {
        fprintf(stderr, "Client error on sending shipment: %d, %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (nb_bytes != msg_size) {
        fprintf(stderr, "Client partial write on sending shipment : %d instead "
                        "of %d .\n", nb_bytes, msg_size);
    }

    /* close connected socket */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr, "Client fails to shutdown the connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Client error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
    freeaddrinfo(server_address_list);
    free(buf);
    if (boxes)
        for (i = 0; i < nb_box; i++)
            free(boxes[i]);

    free(boxes);
    exit(EXIT_FAILURE);    
}
