* Code an HTTP server, listening get requests on 127.0.0.1:5000 for following URLs :

- "/menu/<day>"
The sent response must be only one word fitting the following daily dishes:
monday: burger
tuesday: pizza
wednesday: salad
thursday: burger
friday: pizza
saturday: salad
sunday: pasta

- "/rating/<dish>/<int:rate>"
The answer is free.

You must retrieve pieces of information from provided guest client requests:
- which day guest is asking for ?
- which rate is given to the dish burger by guest ?

====================================================

* Developpez un serveur HTTP répondant sur 127.0.0.1:5000 aux requêtes get sur les urls à paramètres suivants :

- "/menu/<day>"
La réponse renvoyée doit être l'unique mot correspondant aux plats du jour suivants :
monday: burger
tuesday: pizza
wednesday: salad
thursday: burger
friday: pizza
saturday: salad
sunday: pasta

- "/rating/<dish>/<int:rate>"
La réponse est laissée au choix de l'implémentation.

Le serveur de menu doit récupérer les informations suivantes sur les requêtes réalisées par le client guest fourni :
- quel est le jour qui intéresse guest ?
- quel note attribue-t-il au plat burger ? 
