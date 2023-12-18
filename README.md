# adventOfCode2023

Solution compilable under Visual Studio  
Unknown classes I use are defined in helper.hpp  

  following thoses exercices, I list all upgrade I should do in my nested library helper.hpp
  
to do list :  
  - Ajouter des iterateur à la classe Graphe
  - La classe Grid doit tout le temps etre rectangulaire, puis permettre l'insertion d'un element a droite, a gauche, en bas, en haut et s'adapter si l'element ajoute est hors limite. Envisager un heritage de deque et non vector pour les performance d'insertion en debut de ligne.
  - Coder une fonction isDivible(long a, long b) qui retourne la division si elle est entiere, 0 sinon (un algo au niveau du bit serait simple et optimal, car soustraire b à a jusqu'à atteindre 0 est trop couteux)
  - Organiser la lib (elle est presque illisible)
