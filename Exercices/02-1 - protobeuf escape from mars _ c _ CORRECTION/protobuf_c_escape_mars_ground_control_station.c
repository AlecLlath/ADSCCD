#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_tcp.h"
#include "earth_mars.pb-c.h"

const char * health_status[] = {
    [ASTRONAUT__HEALTH_STATUS__GOOD] = "good",
    [ASTRONAUT__HEALTH_STATUS__AILING] = "ailing",
    [ASTRONAUT__HEALTH_STATUS__DEAD] = "dead",
};

struct rescue {
    uint32_t id;
    struct rescue *next;
};

int rescue_add(struct rescue **current_rescues, uint32_t id)
{
    if (*current_rescues == NULL) {
        *current_rescues = malloc(sizeof(struct rescue));
        if (!*current_rescues)
            return ENOMEM;

        (*current_rescues)->id = id;
        (*current_rescues)->next = NULL;
        return 0;
    }

    return rescue_add(&(*current_rescues)->next, id);
}

bool rescue_exist(struct rescue *current_rescues, uint32_t id)
{
    if (current_rescues == NULL)
        return false;

    if (current_rescues->id == id)
        return true;

    return rescue_exist(current_rescues->next, id);
}

void rescue_remove(struct rescue **current_rescues, uint32_t id)
{
    if (*current_rescues == NULL)
        return;

    if ((*current_rescues)->id == id) {
        *current_rescues = (*current_rescues)->next;
        return;
    }

    rescue_remove(&(*current_rescues)->next, id);
}

int send_from_earth(FromEarth *from_earth, int connected_socket)
{
    uint32_t msg_size;
    size_t nb_bytes;
    uint8_t *buf;
    int rc = 0;

    /* alloc and fill buf */
    msg_size = from_earth__get_packed_size(from_earth);
    buf = malloc((size_t)msg_size);
    if (!buf) {
        fprintf(stderr, "Ground unable to alloc buf to send from_earth.\n");
        return ENOMEM;
    }

    nb_bytes = from_earth__pack(from_earth, buf);
    if (nb_bytes != (size_t)msg_size) {
        fprintf(stderr,
                "Ground error when packing from_earth: %u instead of %u .\n",
               nb_bytes, msg_size);
        rc = ENOMEM; goto clean;
    }

    rc = send_protobuf(connected_socket, buf, msg_size);
    if (rc)
        fprintf(stderr,
                "Ground error sending rescue_offer: %d, %s .\n",
                rc, strerror(rc));

clean:
    free(buf);
    return rc;
}

int manage_rescue_demand(RescueDemand *rescue_demand, int connected_socket,
                         struct rescue **current_rescues)
{

    RescueOffer rescue_offer;
    FromEarth from_earth;
    uint32_t i;
    Date date;
    int rc;

    /* Register new rescue_demand */
    if (rescue_exist(*current_rescues, rescue_demand->id)) {
        fprintf(stderr,
                "Ground receives an already existing rescue demand %u .\n",
               rescue_demand->id);
    } else {
        printf("Ground receives a new rescue demand : %u, before %u/%u/%u, %u "
                "astronaut(s).\n",
               rescue_demand->id, rescue_demand->ask_before->year,
               rescue_demand->ask_before->month, rescue_demand->ask_before->day,
               rescue_demand->n_astronaut);
        rc = rescue_add(current_rescues, rescue_demand->id);
        if (rc) {
            fprintf(stderr,
                    "Ground error to add new rescue %u: %d, %s .\n",
                   rescue_demand->id, rc, strerror(rc));
            return rc;
        }
    }

    for (i = 0; i < rescue_demand->n_astronaut; i++) {
        printf("Ground is asked to rescue %s, %s.\n",
                rescue_demand->astronaut[i]->name,
                health_status[rescue_demand->astronaut[i]->health_status]);
    }

    /* prepare rescue_offer */
    rescue_offer__init(&rescue_offer);
    rescue_offer.id = rescue_demand->id;
    rescue_offer.nb_places = rescue_demand->n_astronaut;
    rescue_offer.date = &date;
    date__init(&date);
    date.year = rescue_demand->ask_before->year;
    date.month = rescue_demand->ask_before->month;
    date.day = rescue_demand->ask_before->day;

    /* prepare from_earth */
    from_earth__init(&from_earth);
    from_earth.type_case = FROM_EARTH__TYPE_RESCUE_OFFER;
    from_earth.rescue_offer = &rescue_offer;

    /* send rescue_offer */
    return send_from_earth(&from_earth, connected_socket);
}

void manage_rescue_ack(RescueAck *rescue_ack, int connected_socket,
                      struct rescue **current_rescues)
{
    if (!rescue_exist(*current_rescues, rescue_ack->id)) {
        printf("Ground received an ack about an unknown %u rescue.\n",
               rescue_ack->id);
        return;
    }

    if (rescue_ack->choice == RESCUE_ACK__RESCUE_ACK_CHOICE__REJECTED) {
        printf("Ground received a refusal for rescue %u .\n", rescue_ack->id);
        rescue_remove(current_rescues, rescue_ack->id);
        return;
    }

    printf("Rescue mission %u started.\n", rescue_ack->id);
}

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
    FromMars *from_mars;
    uint8_t *buf = NULL;
    size_t buf_size;
    int rc;
    int i;

    /* Created rescues */
    struct rescue *current_rescues = NULL;

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Ground error on creating the socket: %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* get address */
    rc = getaddrinfo(SERVER_NAME, TCP_PORT, &address_hints,
                     &server_address_list);
    if (rc) {
        fprintf(stderr, "Ground unable to get own adress : %d , %s .\n",
                        rc, gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    if (bind(listening_socket, server_address_list->ai_addr,
             server_address_list->ai_addrlen)) {
        fprintf(stderr, "Ground error on binding: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    freeaddrinfo(server_address_list); server_address_list = NULL;

    /* listen */
    if (listen(listening_socket, NB_CLIENT)) {
        fprintf(stderr, "Ground unable to listen %d clients: %d , %s .\n",
                        NB_CLIENT, errno, strerror(errno));
        goto clean_listening;
    }

    /* accept only one client connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Ground error on accepting socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Ground error on accepting a client address size"
                        " too large: %d accepted, instead of %d .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    printf("Ground Control Station accepts a connection from Mars.\n");

    /* get messages and provide answers */
    while (0 == 0) {
        rc = recv_protobuf(connected_socket, &buf, &buf_size);
        if (rc) {
            fprintf(stderr,
                    "Ground error when receiving from_mars: %d , %s .\n",
                    rc, strerror(rc));
            goto clean;
        }

        /* store from_mars entry */
        from_mars = from_mars__unpack(NULL, buf_size, buf);
        if (!from_mars) {
            fprintf(stderr, "Ground error when unpacking a message.\n");
            goto clean;
        }

        /* free buffer */
        free(buf); buf = NULL;

        switch (from_mars->type_case) {
        case FROM_MARS__TYPE__NOT_SET:
            fprintf(stderr, "Ground received a not set message.");
            break;
        case FROM_MARS__TYPE_RESCUE_DEMAND:
            if (manage_rescue_demand(from_mars->rescue_demand,
                                     connected_socket, &current_rescues)) {
                fprintf(stderr, "Error on managing rescue demand: %d, %s .\n");
                goto clean_unpack;
            }

            break;
        case FROM_MARS__TYPE_RESCUE_ACK:
            manage_rescue_ack(from_mars->rescue_ack, connected_socket,
                              &current_rescues);
            break;
        default:
            fprintf(stderr,
                    "Ground error: receive an unknown from_mars message.\n");
            goto clean_unpack;
        }

        from_mars__free_unpacked(from_mars, NULL);
    }

    /* close sockets */
    if(close(connected_socket)) {
        fprintf(stderr,
                "Ground error on closing connected socket: %d , %s .\n",
                errno, strerror(errno));
    }

    if(close(listening_socket)) {
        fprintf(stderr,
                "Ground error on closing listening socket: %d, %s.\n",
                errno, strerror(errno));
    }

    exit(EXIT_SUCCESS);

clean_unpack:
    from_mars__free_unpacked(from_mars, NULL);
clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    freeaddrinfo(server_address_list);
    free(buf);
    exit(EXIT_FAILURE);    
}
