#!/usr/bin/env python3

import socket

from socket_python_cp import (server_address, server_port, server_output_file,
                              buf_size)

# create the server socket
listening_socket = socket.socket()
listening_socket.bind((server_address, server_port))
listening_socket.listen(1)

# accept the client socket
(connected_socket, client_address) = listening_socket.accept()

# open file
f = open(server_output_file, 'wb')

# receive data
buf = connected_socket.recv(buf_size)
while buf:
    if f.write(buf) != len(buf):
        raise IOError('Server partial write')

    buf = connected_socket.recv(buf_size)

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
listening_socket.close()

# end
print('Server finished to copy.')
