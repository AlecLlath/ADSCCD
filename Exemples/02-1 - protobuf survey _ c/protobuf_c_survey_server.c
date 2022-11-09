/* protobuf_c_survey_server.c */

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
    /* survey result */
    double mean_female_size_in_cm = 0;
    double mean_male_size_in_cm = 0;
    int nb_female = 0;
    int nb_male = 0;

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
    
    /* get survey entries of each client */
    for (i = 0; i < NB_CLIENT; i++) {
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

        /* get all survey entries from one client */
        while (0==0) {
            uint32_t msg_size = 0;
            ssize_t nb_bytes;
            Survey *entry;

            /* get message size*/
            nb_bytes = recv(connected_socket, &msg_size, sizeof(msg_size), 0);
            if (nb_bytes == -1) {
                fprintf(stderr, "Server error when receiving size: %d , %s .\n",
                                errno, strerror(errno));
                goto clean;
            }
        
            /* test close from the client */
            if (nb_bytes == 0) {
                break;
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

            /* read survey entry message */
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

            /* store survey entry */
            entry = survey__unpack(NULL, msg_size, buf);
            if (!entry) {
                fprintf(stderr, "Server error when unpacking a message.\n");
                goto clean;
            }

            if (entry->gender == GENDER__FEMALE) {
                nb_female += 1;
                mean_female_size_in_cm = (mean_female_size_in_cm *
                                            (double)(nb_female - 1) +
                                            (double)entry->size_in_cm) /
                                         ((double) nb_female);
            } else {
                nb_male += 1;
                mean_male_size_in_cm = (mean_male_size_in_cm *
                                            (double)(nb_male - 1) +
                                            (double)entry->size_in_cm) /
                                        ((double) nb_male);
            }
            
            /* free buffer and message */
            free(buf); buf = NULL;
            survey__free_unpacked(entry, NULL);
        }

        /* close connected socket */
        if (close(connected_socket)) {
            fprintf(stderr, "Server error on closing connected socket: %d , %s .\n",
                            errno, strerror(errno));
            goto clean_listening;
        }
    }

    /* close listening socket */
    if (close(listening_socket)) {
        fprintf(stderr, "Server error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* print result */
    fprintf(stdout, "nb female %d .\n", nb_female);
    fprintf(stdout, "nb male %d .\n", nb_male);
    fprintf(stdout, "mean female size in cm: %f .\n", mean_female_size_in_cm);
    fprintf(stdout, "mean male size in cm: %f .\n", mean_male_size_in_cm);
    
    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    freeaddrinfo(server_address_list);
    free(buf);
    exit(EXIT_FAILURE);    
}
