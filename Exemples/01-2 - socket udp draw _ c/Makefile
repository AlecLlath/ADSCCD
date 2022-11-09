all: socket_udp_draw_server socket_udp_draw_client

socket_udp_draw_server: socket_udp_draw_server.c socket_udp_conf.h
	gcc -g -o socket_udp_draw_server socket_udp_draw_server.c

socket_udp_draw_client: socket_udp_draw_client.c socket_udp_conf.h
	gcc -g -o socket_udp_draw_client socket_udp_draw_client.c

clean:
	rm socket_udp_draw_server socket_udp_draw_client
