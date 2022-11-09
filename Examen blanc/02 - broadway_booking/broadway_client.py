#!/usr/bin/env python3

import socket
import struct

from socket_conf import server_address, server_port, one_uint_struct_format, one_uint_nb_bytes

# connection to the server
connected_socket = socket.create_connection((server_address, server_port))

# send number of tickets
connected_socket.sendall(struct.pack(one_uint_struct_format, 2))

#receive global price from the server
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Client partial global price read: {len(buf)} instead of {one_uint_nb_bytes}')

(global_price,) = struct.unpack(one_uint_struct_format, buf)
print(f"The global price for the two tickets is {global_price} .\n")

# send price back 
connected_socket.sendall(struct.pack(one_uint_struct_format, global_price))

#receive booking number from the server
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Client partial global price read: {len(buf)} instead of {one_uint_nb_bytes}')

(booking_number,) = struct.unpack(one_uint_struct_format, buf)
print(f"The booking number is {booking_number} .\n")

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
