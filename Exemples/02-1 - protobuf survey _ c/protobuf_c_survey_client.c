/* protobuf_c_survey_client.c */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

#include "socket_tcp_conf.h"
#include "survey.pb-c.h"

#define MAX_NB_ENTRY 10
#define MAX_SIZE_IN_CM 240

int main(int argc, char **argv)
{
    /* socket variables */
    struct addrinfo address_hints = {0, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL,
                                     NULL};
    struct addrinfo *server_address_list = NULL;
    int connected_socket;
    unsigned int seed;
    int nb_entry;
    uint8_t *buf;
    int rc;
    int i;

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

    /* how many entries */
    seed = time(NULL);
    nb_entry = rand_r(&seed) % MAX_NB_ENTRY;

    /* send each entry */
    for (i = 0; i < nb_entry; i++) {
        uint32_t msg_size;
        ssize_t nb_bytes;
        Survey entry;

        /* init entry */
        survey__init(&entry);
        entry.size_in_cm = rand_r(&seed) % MAX_SIZE_IN_CM;
        if (rand_r(&seed) % 2) {
            entry.gender = GENDER__MALE;
            fprintf(stdout, "Client : %d entry, male, %d cm.\n", 
                    i, entry.size_in_cm);
        } else {
            entry.gender = GENDER__FEMALE;
            fprintf(stdout, "Client : %d entry, female, %d cm.\n", 
                    i, entry.size_in_cm);
        }

        /* alloc buf */
        msg_size = survey__get_packed_size(&entry);
        buf = malloc(msg_size);
        if (!buf) {
            fprintf(stderr, "Client unable to alloc buf.\n");
            goto clean;
        }

        /* fill buf */
        nb_bytes = survey__pack(&entry, buf);
        if (nb_bytes != msg_size) {
            fprintf(stderr, "Client error on packing entry : %d instead of %d "
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

        /* send entry */
        nb_bytes = send(connected_socket, buf, msg_size, 0);
        if (nb_bytes == -1) {
            fprintf(stderr, "Client error on sending entry: %d, %s .\n",
                            errno, strerror(errno));
            goto clean;
        }

        if (nb_bytes != msg_size) {
            fprintf(stderr, "Client partial write on sending entry : %d instead "
                            "of %d .\n", nb_bytes, msg_size);
        }

        /* free entry */
        free(buf); buf = NULL;
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
    exit(EXIT_FAILURE);    
}
