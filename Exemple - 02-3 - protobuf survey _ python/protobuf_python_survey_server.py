""" protobuf_python_survey_server.py """

#!/usr/bin/env python

import socket
import random
import struct

import survey_pb2

SERVER_NAME = 'localhost'
TCP_PORT = 60000
NB_CLIENT = 2

mean_female_size_in_cm = 0.0
mean_male_size_in_cm = 0.0
nb_female = 0
nb_male = 0

# socket creation
# python 3.8 : listening_socket = socket.create_server(('localhost', 60000))
listening_socket = socket.socket()
listening_socket.bind((SERVER_NAME, TCP_PORT))
listening_socket.listen(NB_CLIENT)

# get entries of each client
for i in range(NB_CLIENT):
    # accepting client
    (connected_socket, client_address) = listening_socket.accept()

    # get all survey entries from one client
    while True:
        # get size of the entry message
        size_msg = connected_socket.recv(4)
        if not size_msg:
            break; # end of the connection from client

        (size, ) = struct.unpack('!I', size_msg)

        # get entry message
        entry_message = connected_socket.recv(size)
        if len(entry_message) != size:
            raise IOError('Server partial read for entry message : ' +
                          str(len(entry_message)) + ' instead of ' +
                          str(size))

        entry = survey_pb2.Survey()
        entry.ParseFromString(entry_message)

        # add entry in stats
        if entry.gender == survey_pb2.FEMALE:
            nb_female += 1
            mean_female_size_in_cm = (mean_female_size_in_cm *
                                      float(nb_female - 1) +
                                      float(entry.size_in_cm)) / nb_female
        else:
            nb_male += 1
            mean_male_size_in_cm = (mean_male_size_in_cm *
                                    float(nb_male - 1) +
                                    float(entry.size_in_cm)) / nb_male

    # close connected socket
    connected_socket.close()

# close global socket
listening_socket.close()

# print final stats
print 'nb female: ' + str(nb_female) + ' .'
print 'nb male: ' +str(nb_male) + ' .'
print 'mean female size in cm: ' + str(mean_female_size_in_cm) + ' .'
print 'mean male size in cm: ' + str(mean_male_size_in_cm) + ' .'
    
