#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "ecosys.h"


// TEST DE CREATION ET SUPPRESSION D'ANIMAUX

int main(void) {
  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  float energie = 10;

  srand(time(NULL));

  Animal *a1 = creer_animal(0,1, energie);
  Animal *a2 = creer_animal(5,3, energie);
  Animal *a3 = creer_animal(2,4, energie);
  Animal *a4 = creer_animal(8,3, energie);
  
  liste_proie = a1;
  liste_proie = ajouter_en_tete_animal(liste_proie , a2);
  liste_proie = ajouter_en_tete_animal(liste_proie , a3);
  liste_proie = ajouter_en_tete_animal(liste_proie , a4);
  
  Animal *a5 = creer_animal(2,5, energie);
  Animal *a6 = creer_animal(5,3, energie);
  Animal *a7 = creer_animal(9,2, energie);
  Animal *a8 = creer_animal(7,3, energie);
  
  a5->suivant = a6;
  a6->suivant = a7;
  a7->suivant = a8;

  liste_predateur = a5;
  
  afficher_ecosys(liste_proie, liste_predateur);
  
  ajouter_animal(7,5,energie, &liste_proie);
  
  afficher_ecosys(liste_proie, liste_predateur);
  
  enlever_animal(&liste_predateur, a7);
  
  afficher_ecosys(liste_proie, liste_predateur);
  
  liberer_liste_animaux(liste_proie);
  liberer_liste_animaux(liste_predateur);
  
  return 0;
}
