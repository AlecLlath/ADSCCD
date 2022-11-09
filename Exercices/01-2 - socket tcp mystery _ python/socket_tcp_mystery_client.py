#!/usr/bin/env python3

import socket
import random
import struct

one_int_struct_format = '!I'
one_int_nb_bytes = struct.calcsize(one_int_struct_format)

client_id = 7

# connection to the giver server
connected_socket = socket.create_connection(('localhost', 60000))

#send id
connected_socket.sendall(struct.pack(one_int_struct_format, client_id))

#recv key
buf = connected_socket.recv(one_int_nb_bytes)
if len(buf) != one_int_nb_bytes:
    raise IOError(f'Client partial read: {len(buf)} instead of {one_int_nb_bytes}')

(client_key,) = struct.unpack(one_int_struct_format, buf)

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

# connection to the checker server
connected_socket = socket.create_connection(('localhost', 60001))

#send id
connected_socket.sendall(struct.pack(one_int_struct_format, client_id))

#send key
connected_socket.sendall(struct.pack(one_int_struct_format, client_key))

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
