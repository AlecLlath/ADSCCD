#!/usr/bin/env python3

import socket
import struct

from earth_mars_pb2 import (FromMars, FromEarth, RescueAck, Astronaut)


from socket_tcp import SERVER_NAME, TCP_PORT

def send_protobuf(socket, protobuf_obj):
    msg = protobuf_obj.SerializeToString()
    connected_socket.sendall(struct.pack('!I', len(msg)))
    connected_socket.sendall(msg)

def recv_all(socket, length):
    chunks = []

    msg = bytes()
    while length:
        chunk = socket.recv(length)
        if chunk == b'':
            raise RuntimeError(
                      f"Socket is closed waiting to receive {length} bytes"
                  )

        length -= len(chunk)
        chunks.append(chunk)

    return b''.join(chunks)

def recv_protobuf(socket, protobuf_obj):
    size_msg = recv_all(connected_socket, struct.calcsize('!I'))
    (size, ) = struct.unpack('!I', size_msg)
    msg = recv_all(connected_socket, size)
    protobuf_obj.ParseFromString(msg)

# connection to the server
connected_socket = socket.create_connection((SERVER_NAME, TCP_PORT))

# send rescue demand
from_mars = FromMars()
from_mars.rescue_demand.id = 1
from_mars.rescue_demand.ask_before.year = 2032
from_mars.rescue_demand.ask_before.month = 11
from_mars.rescue_demand.ask_before.day = 27
captain = Astronaut()
captain.name = "Captain"
captain.health_status = Astronaut.AILING
crew_member = Astronaut()
crew_member.name = "Gary"
crew_member.health_status = Astronaut.GOOD
from_mars.rescue_demand.astronaut.extend([captain, crew_member])

# send rescue demand
send_protobuf(connected_socket, from_mars)

# receive rescue offer
from_earth = FromEarth()
recv_protobuf(connected_socket, from_earth)
if from_earth.WhichOneof("type") != "rescue_offer":
    raise RuntimeError("Rescue offer is the only message that I'm ready to recv")

# send rescue ack
from_mars = FromMars()
from_mars.rescue_ack.id = from_earth.rescue_offer.id
from_mars.rescue_ack.choice = RescueAck.ACCEPTED
send_protobuf(connected_socket, from_mars)

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
