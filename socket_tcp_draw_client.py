""" socket_tcp_draw_client.py """

#!/usr/bin/env python3

import socket
import random
import struct

from socket_tcp import server_address, server_port, one_uint_struct_format, one_uint_nb_bytes

# connection to the server
connected_socket = socket.create_connection((server_address, server_port))

# picking a random number
random.seed()
client_draw = random.randint(0, 256)

#receive random number from the server
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Client partial read: {len(buf)} instead of {one_uint_nb_bytes}')

(server_draw,) = struct.unpack(one_uint_struct_format, buf)

# send random number
connected_socket.sendall(struct.pack(one_uint_struct_format, client_draw))

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

# print result
print(f'Client, my draw was: {client_draw}')
print(f'Client, the server draw I received was: {server_draw}')

