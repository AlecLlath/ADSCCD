#!/usr/bin/env python3

import socket
import struct

from olympic_games_registration_pb2 import Registration, Competitor

from socket_conf import REGISTRATION_DESK_IPV4_ADDR, REGISTRATION_DESK_PORT

# connection to the server
connected_socket = socket.create_connection((REGISTRATION_DESK_IPV4_ADDR, REGISTRATION_DESK_PORT))

# build Competitor
competitor = Competitor()
competitor.lastname = "Fix"
competitor.firstname = "John"
competitor.birth_date.year = 1980
competitor.birth_date.month = 7
competitor.birth_date.day = 12

# send Competitor
msg_to_send = competitor.SerializeToString()
connected_socket.sendall(struct.pack('!I', len(msg_to_send)))
connected_socket.sendall(msg_to_send)

# receive Registration
msg_size = connected_socket.recv(struct.calcsize('!I'));
if len(msg_size) != struct.calcsize('!I'):
    raise IOError("Competitor error on receiving msg_size")

(size, ) = struct.unpack('!I', msg_size)
msg_registration = connected_socket.recv(size)
if len(msg_registration) != size:
    raise IOError("Competitor error on receiving registration")

registration = Registration()
registration.ParseFromString(msg_registration)

print(f"My bib number is {registration.bib_number}.")
for contest in registration.contest:
    print(f"Contest name {contest.name}, year {contest.date.year}, month {contest.date.month}, day {contest.date.day}, hour {contest.start_time.hour}, minute {contest.start_time.minute}")






# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

