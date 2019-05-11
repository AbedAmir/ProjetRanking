# ProjetRanking
Abed Amir

Utilisation du pagerank precedent pour initialiser un nouveau calcul.


Comme le Pagerank est calcule chaque mois et que le graphe du web evolue lentement, on peut essayer d'initialiser le vecteur x a la valeur obtenue lors du mois precedent. Il faut simplement faire attention
aux nouveaux sommets et aux sommets disparus.

L'initialisation est la suivante : soit S les sommets du graphe Ht+1 qui ne sont pas dans le graphe Ht, et soit R les sommets du graphe Ht qui ne sont pas dans le graphe Ht+1. S c'est les sommets nouveaux et R les sommets disparus.
  
  * si i est dans le graphe Ht et Ht+1, alors xt+1[i] = xt[i]
  * si i est dans le graphe Ht+1 mais pas dans le graphe Ht, alors xt+1[i] = (somme(xt[j])) / |S|    "tel que 'j' appartient à R"


Etudier l'acceleration de la convergence de l'algorithme des puissance lorsqu'il est initialise comme propose (par rapport a la version du cours ou il est initialise avec e=n). Pour cela, vous proposez des algorithmes de modication des graphes du web dont vous disposez et vous etudierez l'inuence des modications sur l'acceleration.
