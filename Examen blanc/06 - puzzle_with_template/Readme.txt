* Résolvez le puzzle avec modèle en réalisant des requêtes sur le serveur flask puzzle_with_template, dont le binaire est fourni. Ce serveur écoute en http sur l'adresse 127.0.0.1:5000 pour les urls suivants:

- POST sur /puzzle le json suivant: {"piece": <piece number>, "x_abscissa": <x axis coordinate>, "y_ordinate": <y axis coordinate>}
Le serveur renvoie les statuts de réponses suivantes : 200 (la dernière pièce est désormais bien placée et le puzzle est terminé) avec comme contenu le message de félicitation, 201 (la pièce est désormais bien placée), 202 (mauvaise place, la pièce doit être placée à un autre endroit).


Le puzzle est un puzzle de 100 pièces, 10 par 10 . Les pièces sont numérotées de 1 à 100. Lorsque le serveur puzzle_with_template est exécuté, il démarre avec un puzzle vierge, aucune pièce n'est considérée comme placée. Pour indiquer leur place aux pièces, les coordonnées vont de 1 à 10 en x et de 1 à 10 en y.

Vous devez retrouver :
- les coordonnées de la pièce 71, x et y,
- le message de félicitation final renvoyé par le serveur puzzle_with_template avec le code de retour 200.
