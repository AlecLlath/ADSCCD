#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "socket_tcp.h"

static int sendall(int fd, void *buf, size_t len, int flags)
{
    ssize_t nb_bytes;

    while (len) {
        nb_bytes = send(fd, buf, len, flags);
        if (nb_bytes == -1)
            return errno;

        buf = buf + nb_bytes;
        len = len - (size_t) nb_bytes;
    }

    return 0;
}

int send_protobuf(int sockfd, uint8_t *buf, uint32_t len)
{
    int rc;

    /* send size and from_earth */ 
    len = htonl(len);
    rc = sendall(sockfd, &len, sizeof(len), 0);
    if (rc)
        return rc;

    len = ntohl(len);
    rc = sendall(sockfd, buf, (size_t)len, 0);
    if (rc)
        return rc;

    return 0;
}

static int recvall(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t nb_bytes;

    while (len) {
        nb_bytes = recv(sockfd, buf, len, flags);
        if (nb_bytes == -1)
            return errno;

        buf = buf + nb_bytes;
        len = len - (size_t) nb_bytes;
    }

    return 0;
}

int recv_protobuf(int sockfd, uint8_t **buf, size_t *len)
{
    uint32_t msg_size;
    int rc;

    /* recv msg_size */
    rc = recvall(sockfd, &msg_size, sizeof(msg_size), 0);
    if (rc)
        return rc;

    msg_size = ntohl(msg_size);
    *len = (size_t)msg_size;

    /* alloc buf */
    *buf = malloc(*len);
    if (!*buf)
        return ENOMEM;

    rc = recvall(sockfd, *buf, *len, 0);
    if (rc) {
        free(*buf); *buf = NULL;
        return rc;
    }

    return 0;
}

