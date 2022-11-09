* Code a "bowling umpire" socket server that accept the provided "bowling_team" socket client connection.
The address and port are listed in the "socket_conf.h" header file or in the "socket_conf.py" module file.

After being connected, the "bowling_team" client first sends the number of players as one uint32_t .
Then, the client sends the score of each player one by one as one uint32_t.
The client closes afterwards the socket connection.

Your "bowling umpire" must retrieve :
- the number of players,
- the best score,
- the rank of the player who achieves the best score. We assume that the first score sent by the client corresponds to the player with the rank 1.
The second score sent by the client is the one of the player with the rank 2, and so on.

=======================================

* Coder un serveur socket "juge-arbitre de bowling" qui accepte la connexion du client "bowling_team" dont l'exécutable est fourni.
L'adresse et le port de connexion sont listés au sein du fichier d'en-tête "socket_conf.h" ou bien au sein du fichier module "socker_conf.py".

Une fois connecté, le client "bowling_team" commence par envoyer le nombre de joueurs sous la forme d'un uint32_t.
Ensuite, il transmet le score de chacun des joueurs sous la forme d'un uint32_t par joueur.
Enfin, le client ferme la connexion.

Votre serveur "juge-arbitre de bowling" doit retrouver :
- le nombre de joueurs,
- le meilleur score,
- le rang du joueur ayant réalisé le meilleur score. Par convention, on considère que le premier socre transmis par le client est celui du joueur de rang 1, le second est celui du joueur de rang 2 et ainsi de suite.
