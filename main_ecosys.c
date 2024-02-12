#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"


#define NB_TOURS_MAX 200
#define NB_PROIES 1
#define NB_PREDATEURS 7
#define T_WAIT 100000


int main(void) {
	Animal* liste_proie = NULL;
	Animal *liste_predateur = NULL;
	float energie = 1000.0;
	srand(time(NULL));
	int x, y ,i ,j, k;
	int monde[SIZE_X][SIZE_Y];
	for (i=0 ; i< SIZE_X; i++){
		for (j=0; j< SIZE_Y ; j++){
			monde[i][j] = 0;	
		}
	}
	
	/* TEST DE DEPLACEMENT ET REPRODUCTION
	Animal *a1 = creer_animal(0,0, energie);
	a1 -> dir[0] = -1;
	a1 -> dir[1] = -1;
	liste_proie = a1;
	
	
	afficher_ecosys(liste_proie,NULL);
	for (int i = 0 ; i<9 ; i++){
		bouger_animaux(liste_proie);
	}
	afficher_ecosys(liste_proie,NULL);
	bouger_animaux(liste_proie);
	afficher_ecosys(liste_proie,NULL);
	
	for (int i = 0 ; i<5 ; i++){
		reproduce(&liste_proie, p_reproduce_proie);
	}
	afficher_ecosys(liste_proie,NULL);
	*/
	
	/* TEST RAFRAICHISSEMENT DE L'ECOSYSTEME
  	k = 0;
  	while((k< NB_TOURS_MAX) && (compte_animal_rec(liste_proie) !=0)){
		rafraichir_predateurs(&liste_predateur, &liste_proie);
		rafraichir_proies(&liste_proie, monde);
		rafraichir_monde(monde);
		k++;
	}
	*/

	// CREATION LISTE PROIES ET PREDATEURS
	for (i = 0; i < NB_PROIES; i++) {
  		x = (int)(rand() % SIZE_X);
  		y = (int)(rand() % SIZE_Y);
  		ajouter_animal(x, y, energie, &liste_proie);
  	}
  	for (i = 0; i < NB_PREDATEURS; i++) {
  		x = (int)(rand() % SIZE_X);
  		y = (int)(rand() % SIZE_Y);
  		ajouter_animal(x, y, energie, &liste_predateur);
  	}
  	
  	//AFFICHAGE ET ECRITURE DE L'ECOSYSTEME INITIAL
  	afficher_ecosys(liste_proie,liste_predateur);
  	ecrire_ecosys("ecosysD.txt", liste_predateur, liste_proie); 
	
	
	//RAFRAICHISSEMENT ET ECRITURE DES RESULTATS DES TOURS DE L'ECOSYSTEME PERMETTANT DE DONNER LE GRAPHIQUE
	FILE *f = fopen("Evol_pop.txt", "w");
	if (f == NULL){
		printf("Erreur ouverture fichier\n");
	}
	k = 0;
	//fprintf(f, "Tour Nbp NbP\n");
	fprintf(f, "%d %d %d\n", k, compte_animal_rec(liste_proie) , compte_animal_rec(liste_predateur));
  	while((k< NB_TOURS_MAX) && ((compte_animal_rec(liste_proie) !=0) || compte_animal_rec(liste_predateur) !=0)){
		rafraichir_predateurs(&liste_predateur, &liste_proie);
		rafraichir_proies(&liste_proie, monde);
		rafraichir_monde(monde);
		fprintf(f, "%d %d %d\n", k+1, compte_animal_rec(liste_proie) , compte_animal_rec(liste_predateur));
		afficher_ecosys(liste_proie, liste_predateur);
		usleep(T_WAIT);
		k++;
	}
	fclose(f);
	
	
	//AFFICHAGE DU RESULTAT DE L'ECOSYSTEME FINAL
	if ((compte_animal_rec(liste_proie) ==0) && ((compte_animal_rec(liste_predateur) ==0))) {
		printf("L'ecosysteme est mort... au tour %d\n" , k);
	} 
	ecrire_ecosys("ecosysF.txt", liste_predateur, liste_proie);
	
	
	//SUPPRESSION MEMOIRE DES LISTES
	liberer_liste_animaux(liste_proie);
	liberer_liste_animaux(liste_predateur);
	
	return 0;
}

