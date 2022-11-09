#!/usr/bin/env python3

import socket
import random
import struct

from socket_conf import vault_tcp_port, vault_ipv4_addr, one_uint_struct_format, one_uint_nb_bytes, whisperer_destination_ipv4_addr as server_address, whisperer_connection_port as server_port


##########################################################################################
##########################################################################################
# Obtenir le mdp

listening_socket = socket.socket()
listening_socket.bind((server_address, server_port))
listening_socket.listen(1)

# accepting client
(connected_socket, client_address) = listening_socket.accept()

#receive taille
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Server partial read: {len(buf)} instead of {one_uint_nb_bytes}')

(TailleMdp,) = struct.unpack(one_uint_struct_format, buf)
print(TailleMdp)

#receive mdp
mdp = connected_socket.recv(TailleMdp)
print('Le mdp est :' + str(mdp)) # En fait il envoie le truc comme il faut donc faut pas le unpack


# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
listening_socket.close()

##########################################################################################
##########################################################################################

# Vault
# connection to the server
connected_socket = socket.create_connection((vault_ipv4_addr, vault_tcp_port))

# envoie la taille
connected_socket.sendall(struct.pack(one_uint_struct_format, TailleMdp))

# envoie le mdp
connected_socket.sendall(mdp)

#receive from the server
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Client partial read: {len(buf)} instead of {one_uint_nb_bytes}')

(server_draw,) = struct.unpack(one_uint_struct_format, buf)


# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

# print result
print(f'Client, the vault draw I received was: {server_draw}')


