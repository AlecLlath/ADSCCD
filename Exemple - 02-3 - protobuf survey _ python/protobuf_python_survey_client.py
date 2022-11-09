""" protobuf_python_survey_client.py """

#!/usr/bin/env python

import socket
import random
import struct

import survey_pb2

SERVER_NAME = 'localhost'
TCP_PORT = 60000
MAX_NB_ENTRY = 10
MAX_SIZE_IN_CM = 240

# connection to the server
connected_socket = socket.create_connection((SERVER_NAME, TCP_PORT))

# how many entries into the survey
random.seed()
nb_entry = random.randint(0, MAX_NB_ENTRY)

for i in range(nb_entry):
    entry = survey_pb2.Survey()
    entry.gender = random.choice((survey_pb2.FEMALE,
                                  survey_pb2.MALE))
    entry.size_in_cm = random.randint(0, MAX_SIZE_IN_CM)
    entry_msg = entry.SerializeToString()
    # send message size
    connected_socket.sendall(struct.pack('!I', len(entry_msg)))
    # send message
    connected_socket.sendall(entry.SerializeToString())
    print 'Client: ' + str(entry)

# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

