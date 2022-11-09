import socket
import struct

from socket_tcp import server_address, server_port, one_uint_struct_format, one_uint_nb_bytes

# socket creation
listening_socket = socket.socket()
listening_socket.bind((server_address, server_port))
listening_socket.listen(1)

# accepting client
(connected_socket, client_adddress) = listening_socket.accept()

# receive number of players
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Server partial read : {len(buf)} instead of {one_uint_nb_bytes}')
(nb_players,) = struct.unpack(one_uint_struct_format, buf)

# receive scores
best_score = 0
rank_best_score = 0
for i in range(0, nb_players):
    buf = connected_socket.recv(one_uint_nb_bytes)
    if len(buf) != one_uint_nb_bytes:
        raise IOError(f'Server partial read : {len(buf)} instead of {one_uint_nb_bytes}')
    (score,) = struct.unpack(one_uint_struct_format, buf)
    if score > best_score:
        best_score = score
        rank_best_score = i


# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
listening_socket.close()

# print result
print(f'Number of players : {nb_players}')
print(f'Number of player : {best_score}')
print(f'Number of player : {rank_best_score}')