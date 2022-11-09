all: socket_tcp_draw_server socket_tcp_draw_client

socket_tcp_draw_server: socket_tcp_draw_server.c socket_tcp_conf.h
	gcc -g -o socket_tcp_draw_server socket_tcp_draw_server.c

socket_tcp_draw_client: socket_tcp_draw_client.c socket_tcp_conf.h
	gcc -g -o socket_tcp_draw_client socket_tcp_draw_client.c

clean:
	rm socket_tcp_draw_server socket_tcp_draw_client
