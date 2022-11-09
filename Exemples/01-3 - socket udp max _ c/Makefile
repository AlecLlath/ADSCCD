all: socket_udp_max_server socket_udp_max_client

socket_udp_max_server: socket_udp_max_server.c socket_udp_conf.h
	gcc -g -o socket_udp_max_server socket_udp_max_server.c

socket_udp_max_client: socket_udp_max_client.c socket_udp_conf.h
	gcc -g -o socket_udp_max_client socket_udp_max_client.c

clean:
	rm socket_udp_max_server socket_udp_max_client
