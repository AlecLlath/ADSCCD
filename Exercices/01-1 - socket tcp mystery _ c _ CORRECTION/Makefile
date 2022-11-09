all: socket_tcp_mystery_giver socket_tcp_mystery_checker \
     socket_tcp_mystery_client

socket_tcp_mystery_giver: socket_tcp_mystery_giver.c socket_tcp_conf.h \
                          mystery.h mystery.c
	gcc -g -o socket_tcp_mystery_giver socket_tcp_mystery_giver.c \
        mystery.c

socket_tcp_mystery_checker: socket_tcp_mystery_checker.c socket_tcp_conf.h \
                            mystery.h mystery.c
	gcc -g -o socket_tcp_mystery_checker socket_tcp_mystery_checker.c \
        mystery.c

socket_tcp_mystery_client: socket_tcp_mystery_client.c socket_tcp_conf.h
	gcc -g -o socket_tcp_mystery_client socket_tcp_mystery_client.c

clean:
	rm socket_tcp_mystery_giver socket_tcp_mystery_checker \
    socket_tcp_mystery_client
