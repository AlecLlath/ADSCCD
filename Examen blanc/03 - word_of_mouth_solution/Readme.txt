* Your code must :
- first accept the connection from the "password_whisperer" and receives first the length in bytes of the password as one uint32_t and then the bytes of the password (ascii),
- then connects to the "vault" socket server to send first the length in bytes of the password as one uint32_t and then the bytes of the password,
- finally, the "vault" answers the value of the treasure as one uint32_t .

The address an port to listen from the "password_whisperer" and to connect to the "vault" are listed in the "socket_conf.h" header file or in the socket_conf.py" module file.

As answer, provide :
- the password,
- the value of the treasure.

=======================================

* Votre code doit :
- premièrement accepter la connexion de la part du "password_whisperer" (chuchoteur de mot de passe) et recevoir de sa part tout d'abord un entier uint32_t indiquant la longueur
en octet du mot de passe, puis les octets de ce mot de passe (ascii),
- ensuite se connecter au "vault" (coffre fort) pour lui transmettre le mot de passe d'abord par un uint32_t indiquant le nombre d'octets puis les octets du mot de passe,
- enfin le "vault", coffre fort, répond en transmettant la valeur du trésor sous la forme d'un entier uint32_t.

L'adresse et le port pour recevoir la connexion en provenance du "password_whisperer" (chuchoteur de mot de passe) et pour se connecter au "vault" (coffre fort) sont listés
dans le fichier d'en-tête "socket_conf.h" ou dans le fichier module "socket_conf.py" .

Comme réponse à l'exercice, vous devez fournir :
- le mot de passe transmis,
- la valeur du trésor découverte.
