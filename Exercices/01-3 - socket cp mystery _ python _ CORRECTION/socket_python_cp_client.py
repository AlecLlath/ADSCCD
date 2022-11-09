#!/usr/bin/env python3

import socket

from socket_python_cp import (server_address, server_port, client_input_file,
                              buf_size)

# create and connect the socket
connected_socket = socket.create_connection((server_address, server_port))

# open file
f = open(client_input_file, 'rb')

buf = f.read(buf_size)
while len(buf):
    connected_socket.sendall(buf)
    buf = f.read(buf_size)

# close socket
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

# end
print('Client finished to read and to send.')
