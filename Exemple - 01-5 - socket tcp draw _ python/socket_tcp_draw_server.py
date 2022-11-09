""" socket_tcp_draw_server.py """

#!/usr/bin/env python3

import socket
import random
import struct

from socket_tcp import server_address, server_port, one_uint_struct_format, one_uint_nb_bytes

# socket creation
# python 3.8 : listening_socket = socket.create_server(('localhost', 60000))
listening_socket = socket.socket()
listening_socket.bind((server_address, server_port))
listening_socket.listen(1)

# accepting client
(connected_socket, client_address) = listening_socket.accept()

# picking a random number
random.seed()
server_draw = random.randint(0, 256)

# send random number
connected_socket.sendall(struct.pack(one_uint_struct_format, server_draw))

#receive random number
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Server partial read: {len(buf)} instead of {one_uint_nb_bytes}')

(client_draw,) = struct.unpack(one_uint_struct_format, buf)

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
listening_socket.close()

# print result
print(f'Server, my draw was: {server_draw}')
print(f'Server, the client draw I received was: {client_draw}')

