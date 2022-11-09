#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_conf.h"
#include "olympic_games_registration.pb-c.h"

#define CHAR_OBFUSCATION(c) ((c) ^ ((char) 42))
#define CO(c) CHAR_OBFUSCATION(c)

static inline char *string_obfuscation(char *s)
{
    size_t i;

    for (i = 0; i < strlen(s); i++) {
        s[i] = s[i] ^ ((char) 42);
    }

    return s;
}

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    StartTime start_time_array[3];
    Contest *contest_p_array[3];
    Registration registration;
    Contest contest_array[3];
    Competitor *competitor;
    Date date_array[3];
    int listening_socket;
    int connected_socket;
    uint8_t *buf = NULL;
    uint32_t msg_size;
    ssize_t nb_bytes;
    int rc;
    int i;

    char contest_1[] = {CO('r'), CO('u'), CO('n'), CO('n'), CO('i'), CO('n'), CO('g'), '\0'};
    char contest_2[] = {CO('j'), CO('u'), CO('m'), CO('p'), CO('i'), CO('n'), CO('g'), '\0'};
    char contest_3[] = {CO('t'), CO('h'), CO('r'), CO('o'), CO('w'), CO('i'), CO('n'), CO('g'), '\0'};

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Desk error on creating the socket: %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(REGISTRATION_DESK_PORT);
    if (!inet_aton(REGISTRATION_DESK_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Desk socket adress is not valid : %s \n",
                        REGISTRATION_DESK_IPV4_ADDR);
        goto clean_listening;
    }
    if (bind(listening_socket, (struct sockaddr *)&server_address,
             sizeof(server_address))) {
        fprintf(stderr, "Desk error on binding: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* listen */
    if (listen(listening_socket, 1)) {
        fprintf(stderr, "Desk unable to listen %d clients: %d , %s .\n",
                        1, errno, strerror(errno));
        goto clean_listening;
    }

    printf("Desk ready to accept competitor connection.\n");

    /* accept connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Desk error on accepting socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Desk error on accepting a client address size too "
                        "large: %d accepted, instead of %ld .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    printf("Desk accepted competitor connection.\n");

    /* get message size*/
    nb_bytes = recv(connected_socket, &msg_size, sizeof(msg_size), 0);
    if (nb_bytes == -1) {
        fprintf(stderr, "Desk error when receiving size: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    /* check partial read */
    if (nb_bytes != sizeof(msg_size)) {
        fprintf(stderr, "Desk got a partial read on getting size: %ld "
                        "instead of %ld .\n", nb_bytes, sizeof(msg_size));
        goto clean;
    }

    msg_size = ntohl(msg_size);

    printf("Desk received the message size : %u .\n", msg_size);

    /* alloc message buf */
    buf = malloc(msg_size);
    if (!buf) {
        fprintf(stderr, "Desk error on allocating message buf of "
                        "size: %d .\n", msg_size);
        goto clean;
    }

    /* read competitor message */
    nb_bytes = recv(connected_socket, buf, msg_size, 0);
    if (nb_bytes == -1) {
        fprintf(stderr, "Desk error when receiving message: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    /* check partial read */
    if (nb_bytes != msg_size) {
        fprintf(stderr, "Desk got a partial read on message: %ld "
                        "instead of %d .\n", nb_bytes, msg_size);
        goto clean;
    }

    printf("Desk received the competitor message .\n");

    competitor = competitor__unpack(NULL, msg_size, buf);
    if (!competitor) {
        fprintf(stderr, "Desk error when unpacking a message.\n");
        goto clean;
    }

    /* free buffer */
    free(buf); buf = NULL;

    printf("Desk successfully unpacked the Competitor message.\n");

    /* Check competitor is the good one */
    if (strcmp("Fix", competitor->lastname)) {
        fprintf(stderr, "Competitor lastname must be Fix .\n");
        competitor__free_unpacked(competitor, NULL);
        goto clean;
    }

    if (strcmp("John", competitor->firstname)) {
        fprintf(stderr, "Competitor firstname must be John .\n");
        competitor__free_unpacked(competitor, NULL);
        goto clean;
    }

    if (competitor->birth_date->year != 1980) {
        fprintf(stderr, "Competitor year of birth must be 1980 .\n");
        competitor__free_unpacked(competitor, NULL);
        goto clean;
    }

    if (competitor->birth_date->month != 7) {
        fprintf(stderr, "Competitor month of birth must be 7 .\n");
        competitor__free_unpacked(competitor, NULL);
        goto clean;
    }

    if (competitor->birth_date->day != 12) {
        fprintf(stderr, "Competitor day of birth must be 12 .\n");
        competitor__free_unpacked(competitor, NULL);
        goto clean;
    }
     
    competitor__free_unpacked(competitor, NULL);

    printf("Desk received the good Competitor message.\n");

    /* build registration */
    date__init(&date_array[0]);
    date_array[0].year = 2024; date_array[0].month = 8; date_array[0].day = 11;
    date__init(&date_array[1]);
    date_array[1].year = 2024; date_array[1].month = 7; date_array[1].day = 13;
    date__init(&date_array[2]);
    date_array[2].year = 2024; date_array[2].month = 7; date_array[2].day = 13;
    start_time__init(&start_time_array[0]);
    start_time_array[0].hour = 11; start_time_array[0].minute = 32;
    start_time__init(&start_time_array[1]);
    start_time_array[1].hour = 17; start_time_array[1].minute = 12;
    start_time__init(&start_time_array[2]);
    start_time_array[2].hour = 9; start_time_array[1].minute = 47;
    contest__init(&contest_array[0]);
    contest_array[0].name = string_obfuscation(contest_1);
    contest_array[0].date = &date_array[0];
    contest_array[0].start_time = &start_time_array[0];
    contest__init(&contest_array[1]);
    contest_array[1].name = string_obfuscation(contest_2);
    contest_array[1].date = &date_array[1];
    contest_array[1].start_time = &start_time_array[1];
    contest__init(&contest_array[2]);
    contest_array[2].name = string_obfuscation(contest_3);
    contest_array[2].date = &date_array[2];
    contest_array[2].start_time = &start_time_array[2];
    contest_p_array[0] = &contest_array[0];
    contest_p_array[1] = &contest_array[1];
    contest_p_array[2] = &contest_array[2];
    registration__init(&registration);
    registration.bib_number = 123456;
    registration.n_contest = 3;
    registration.contest = &contest_p_array[0];

    /* send size and registration */
    msg_size = registration__get_packed_size(&registration);
    buf = malloc(msg_size);
    if (!buf) {
        fprintf(stderr, "Desk unable to alloc buf.\n");
        goto clean;
    }

    /* fill buf */
    nb_bytes = registration__pack(&registration, buf);
    if (nb_bytes != msg_size) {
        fprintf(stderr, "Desk error on packing registration : %ld instead of %d "
                         ".\n", nb_bytes, msg_size);
        goto clean;
    }

    /* send size */
    msg_size = htonl(msg_size);
    nb_bytes = send(connected_socket, &msg_size, sizeof(msg_size), 0);
    if (nb_bytes == -1) {
        fprintf(stderr, "Desk error on sending msg size: %d, %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    msg_size = ntohl(msg_size);

    if (nb_bytes != sizeof(msg_size)) {
        fprintf(stderr, "Desk partial write on sending size : %ld instead "
                        "of %ld .\n", nb_bytes, sizeof(msg_size));
    }

    printf("Desk sent the message size.\n");

    /* send shipment */
    nb_bytes = send(connected_socket, buf, msg_size, 0);
    if (nb_bytes == -1) {
        fprintf(stderr, "Desk error on sending shipment: %d, %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (nb_bytes != msg_size) {
        fprintf(stderr, "Desk partial write on sending registration : %ld instead "
                        "of %d .\n", nb_bytes, msg_size);
    }

    printf("Desk sent the Registration message.\n");

    /* shutdown connected_socket */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr, "Desk error on shutdown socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }
    
    /* close connected socket */
    if(close(connected_socket)) {
        fprintf(stderr, "Desk error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* close listening socket */
    if(close(listening_socket)) {
        fprintf(stderr, "Desk error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Desk closed sockets.\n");

    exit(EXIT_SUCCESS);

clean:
    shutdown(connected_socket, SHUT_RDWR);
    close(connected_socket);
clean_listening:
    close(listening_socket);
    free(buf);
    exit(EXIT_FAILURE);    
}
