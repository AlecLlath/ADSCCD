* Code a socket client that connects the provided "broadway_shop" socket server to buy two tickets.
The address and the port of the connection are listed in the "socket.conf" header file or in the "socket_conf.py" module file.

After having set the connection, the client must send first to the server the number of tickets to buy (2) as a uint32_t.
Then, the server answer to the client the corresponding price to the both places as a uint32_t.
Then, the client must send back the price as a uint32_t to the server.
Finally, after receiving this price back from the client, the server send the booking number to the client.

The client must retrieve :
- the global price for the both place,
- the booking number.

=======================================================

* Développer un client socket qui se connecte sur le serveur socket fourni "broadway_shop".
L'adresse et le port de connexion sont indiqués au sein du fichier d'en-tête "socket_conf.c" ou au sein du fichier module "socket.py" .

Après s'être connecté, le client doit envoyer au serveur le nombre de tickets (2) qu'il souhaite acheter sous la forme d'un uint32_t. 
uis, le serveur lui répond le prix global correspondant à ces 2 places, également sous la forme d'un uint32_t.
Le client doit ensuite renvoyer au serveur le prix reçu, toujours sous la forme d'un uint32_t.
Pour finir, une fois le prix reçu en retour de la part du client, le serveur transmet au client sous la forme d'un uint32_t le numéro de réservation.

Le client doit récupérer :
- le prix global des deux places,
- le numéro de la réservation.
