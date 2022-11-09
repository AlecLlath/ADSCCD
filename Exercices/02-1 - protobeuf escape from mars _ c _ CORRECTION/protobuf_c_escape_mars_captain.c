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

int send_from_mars(FromMars *from_mars, int connected_socket)
{
    uint32_t msg_size;
    size_t nb_bytes;
    uint8_t *buf;
    int rc = 0;

    /* alloc and fill buf */
    msg_size = from_mars__get_packed_size(from_mars);
    buf = malloc(msg_size);
    if (!buf) {
        fprintf(stderr, "Captain unable to alloc buf to send from_mars.\n");
        return ENOMEM;
    }

    nb_bytes = from_mars__pack(from_mars, buf);
    if (nb_bytes != (size_t)msg_size) {
        fprintf(stderr,
                "Captain error when packing from_mars: %u instead of %u .\n",
               nb_bytes, msg_size);
        rc = ENOMEM; goto clean;
    }

    rc = send_protobuf(connected_socket, buf, msg_size);
    if (rc) {
        fprintf(stderr,
                "Captain error on sending from_mars: %d, %s .\n",
                rc, strerror(rc));
        goto clean;
    }

clean:
    free(buf);
    return rc;
}

int manage_rescue_offer(RescueOffer *rescue_offer, int connected_socket)
{
    RescueAck rescue_ack;
    FromMars from_mars;
    uint32_t i;
    int rc;

    printf("Captain receives and acks the rescue_offer %u .\n",
           rescue_offer->id);

    /* prepare rescue_ack */
    rescue_ack__init(&rescue_ack);
    rescue_ack.id = rescue_offer->id;
    rescue_ack.choice = RESCUE_ACK__RESCUE_ACK_CHOICE__ACCEPTED;

    /* prepare from_mars */
    from_mars__init(&from_mars);
    from_mars.type_case = FROM_MARS__TYPE_RESCUE_ACK;
    from_mars.rescue_ack = &rescue_ack;

    /* send rescue_offer */
    return send_from_mars(&from_mars, connected_socket);
}

int main(int argc, char **argv)
{
    /* socket variables */
    struct addrinfo address_hints = {0, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL,
                                     NULL};
    struct addrinfo *server_address_list = NULL;
    int connected_socket;
    Astronaut captain, crew_member;
    RescueDemand rescue_demand;
    FromEarth *from_earth;
    FromMars from_mars;
    Date ask_before;
    uint8_t *buf = NULL;
    size_t buf_size;
    int rc;

    /* creating a tcp-IPv4 socket*/
    connected_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connected_socket == -1) {
        fprintf(stderr,
                "Captain error on creating the socket: %d, %s.\n",
                errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* get address */
    rc = getaddrinfo(SERVER_NAME, TCP_PORT, &address_hints,
                     &server_address_list);
    if (rc) {
        fprintf(stderr,
                "Captain unable to get server address : %d , %s .\n",
                rc, gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    /* connect socket */
    if (connect(connected_socket, server_address_list->ai_addr,
             server_address_list->ai_addrlen)) {
        fprintf(stderr,
                "Captain error on connect: %d , %s .\n",
                errno, strerror(errno));
        freeaddrinfo(server_address_list); server_address_list = NULL;
        goto clean;
    }

    freeaddrinfo(server_address_list); server_address_list = NULL;
    printf("Captain connects to Ground Control Station.\n");

    /* prepare rescue_demand */
    date__init(&ask_before);
    ask_before.year = 2032;
    ask_before.month = 11;
    ask_before.day = 27;
    astronaut__init(&captain);
    captain.name = "Captain";
    captain.health_status = ASTRONAUT__HEALTH_STATUS__AILING;
    astronaut__init(&crew_member);
    crew_member.name = "Gary";
    captain.health_status = ASTRONAUT__HEALTH_STATUS__GOOD;
    rescue_demand__init(&rescue_demand);
    rescue_demand.id = 1;
    rescue_demand.ask_before = &ask_before;
    rescue_demand.n_astronaut = 2;
    rescue_demand.astronaut = malloc(sizeof(Astronaut *) * 2);
    if (!rescue_demand.astronaut) {
        fprintf(stderr,
                "Captain unable to alloc astronaut list for rescue_demand.\n");
        goto clean;
    }

    rescue_demand.astronaut[0] = &captain;
    rescue_demand.astronaut[1] = &crew_member;
    from_mars__init(&from_mars);
    from_mars.type_case = FROM_MARS__TYPE_RESCUE_DEMAND;
    from_mars.rescue_demand = &rescue_demand;
    
    /* send rescue_demand */
    rc = send_from_mars(&from_mars, connected_socket);
    if (rc) {
        fprintf(stderr,
                "Captain error when sending rescue_demand: %d, %s .\n",
                rc, strerror(rc));
        goto clean;
    }

    /* recv from_earth buf */
    rc = recv_protobuf(connected_socket, &buf, &buf_size);
    if (rc) {
        fprintf(stderr,
                "Captain error when receiving: %d , %s .\n",
                rc, strerror(rc));
        goto clean;
    }

    from_earth = from_earth__unpack(NULL, buf_size, buf);
    if (!from_earth) {
        fprintf(stderr, "Captain error when unpacking from_earth.\n");
        goto clean;
    }

    if (from_earth->type_case != FROM_EARTH__TYPE_RESCUE_OFFER) {
        fprintf(stderr,
                "Captain received an unknowkn type message from earth: %d.\n",
                from_earth->type_case);
        from_earth__free_unpacked(from_earth, NULL);
        goto clean;
    }

    rc = manage_rescue_offer(from_earth->rescue_offer, connected_socket);
    from_earth__free_unpacked(from_earth, NULL);
    if (rc) {
        fprintf(stderr,
                "Captain unabled to manage rescue_offer: %d , %s .\n",
                rc, strerror(rc));
        goto clean;
    }

    /* close socket */
    if(close(connected_socket)) {
        fprintf(stderr,
                "Captain error on closing connected socket: %d , %s .\n",
                errno, strerror(errno));
    }

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
    freeaddrinfo(server_address_list);
    free(buf);
    exit(EXIT_FAILURE);    
}
