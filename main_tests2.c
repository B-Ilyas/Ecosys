#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"


int main(void) {
  	int i;
  	Animal *liste_proie = NULL;
  	Animal *liste_predateur = NULL;
  	float energie = 10.00000;
  	int x, y;

  	srand(time(NULL));

	//CREATION DES LISTES
  	for (i = 0; i < 20; i++) {
  		x = (int)(rand() % SIZE_X);
  		y = (int)(rand() % SIZE_Y);
  		ajouter_animal(x, y, energie, &liste_proie);
  	} 
  	for (i = 0; i < 20; i++) {
  		x = (int)(rand() % SIZE_X);
  		y = (int)(rand() % SIZE_Y);
  		ajouter_animal(x, y, energie, &liste_predateur);
  	}
  	printf("Affichage ecosysteme %d predateurs et %d proies aléatoire sur un environnement de longueur %d sur une largeur de %d\n",compte_animal_rec(liste_predateur), compte_animal_rec(liste_proie), SIZE_Y , SIZE_X);
  	afficher_ecosys(liste_proie,liste_predateur);
  
  // TEST D'ECRITURE ET DE LECTURE DU FICHIER 
  ecrire_ecosys("ecosys.txt", liste_predateur, liste_proie);
  Animal* liste_proie_lu = NULL;
  Animal* liste_predateur_lu = NULL;
  lire_ecosys("ecosys.txt", &liste_predateur_lu, &liste_proie_lu);
  
  //TEST DE RESSEMBLANCE ENTRE LES 2 ECOSYSTEMES
  printf("Affichage du meme ecosysteme lu par un fichier:\n");
  afficher_ecosys(liste_proie_lu,liste_predateur_lu);
  
  
  //SUPPRESSION MEMOIRES DES LISTES
  liberer_liste_animaux(liste_proie);
  liberer_liste_animaux(liste_predateur);
  liberer_liste_animaux(liste_proie_lu);
  liberer_liste_animaux(liste_predateur_lu);
  
 
  return 0;
}


