* Code an olympic games competitor (lastname Fix, firstname John, birth date the 12th of july, 1980) who connects to the registration desk (the registration_desk server binary is provided). The address and port of the connection are available in the "socket_conf.h" header file or in the "socket_conf.py" module file.

Competitor and registration desk exchanges protobuf messages defined in the provided olympic_games_registration.proto . Each protobuf message is preceded by a message of only one uint32_t that announces the size of the protobuf message that is coming.

First John Fix must send his Competitor message to the registration desk. After checking this competitor message, the registration desk answers by sending a Registration message to the competitor.

The competitor must retrieve :
- his bib number,
- the name of the first starting contest,
- the start hour of the contest with the last date.

* Développez un concurrent des jeux olympiques (Nom Fix, Prénom John, né le 12 juillet 1980) qui pointe son arrivée au village olympique en se connectant au bureau d'enregistrement. Le binaire registration_desk est fourni. L'adresse et le port de connexion sont fournis au sein du fichier d'en tête "socket_conf.h" ou dans le fichier module "socket_conf.py".

Le concurrent et le bureau d'enregistrement communique en échangeant des messages protobuf définis dans olympic_games_registration.proto . Chaque message protobuf doit être précédé d'un message ne contenant qu'un unique uint32_t qui annonce la taille du message protobuf à venir.

En premier, John Fix doit faire parvenir son message Competitor au bureau d'enregistrement. Après avoir vérifier ce message, le bureau d'enregistrement répond un message Registration à destination du concurrent.

Le concurrent doit retrouver :
- son numéro de dossard ("bib number"),
- le nom de l'épreuve ("contest") qui débutera en premier,
- l'heure de début de l'épreuve dont la date sera la dernière.
