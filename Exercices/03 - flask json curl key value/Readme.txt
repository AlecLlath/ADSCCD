Implémenter un clé/valeur client/server HTTP.

Les clés et les valeurs seront des strings.


1) Implémenter un server python Flask répondant aux requêtes suivantes :

PUT sur /put en recevant un json constitué d'un dictionnaire de clé/valeur

(Les clés et les valeurs étant des strings.)

GET sur /get?key=<la_cle> et renvoyant une erreur si la clé n'existe pas ou bien
la valeur

POST /post en envoyant un json constitué d'un tableau de clé et recevant en
réponse un json dictionnaire de toutes les paires clé/valeur existantes

2) Implémenter un client libcurl en C effectuant une requête de put positionnant
deux paires clé/valeur et effectuant au minimum un get et un post (ce dernier
avec un tableau contenant des clés existantes et des clés inexistantes, et en
analysant dans la réponse pour déterminer les clés qui étaient existantes et
celles qui ne l'étaient pas)
