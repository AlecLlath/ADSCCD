This code is a simple example of using sockets in C.

A client sends the identity of his choice (an unsigned integer on 4 bytes
(uint32_t)) to the mystery giver server which replies to the client the mystery
key corresponding with the identity given by the client (an unsigned integer on
4 bytes (uint32_t)). The mystery key is shared between the giver and the
checker.

Then, the client sends to the checker the two unsigned integers on 4 bytes
(uint32_t), first its identity number, then the corresponding mystery key.
The checker server prints on its output the mystery validation.

The client must use AF_INET/SOCK_STREAM socket to communicate with the both
servers. IP address and port of the both servers are in socket_tcp_conf.h .

Mystery Giver and Checker servers are in "ELF 64-bit LSB executable, x86-64"
format.
