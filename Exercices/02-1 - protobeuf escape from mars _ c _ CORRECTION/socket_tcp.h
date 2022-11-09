#ifndef SOCKET_TCP_CONF_H
#define SOCKET_TCP_CONF_H

#include <stdint.h>
#include <stdlib.h>

#define SERVER_NAME "localhost"
#define TCP_PORT "60000"
#define NB_CLIENT 1

int send_protobuf(int sockfd, uint8_t *buf, uint32_t len);
int recv_protobuf(int sockfd, uint8_t **buf, size_t *len);

#endif /* SOCKET_TCP_CONF_H */
