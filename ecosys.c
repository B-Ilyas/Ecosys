#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"
#include <time.h>

/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.2; //favorise la proie ou le predateur en fct de la situation
float p_reproduce_proie=0.3;
float p_reproduce_predateur=0.3; 
int temps_repousse_herbe=-10; //favorise ou pas la proie

/* PARTIE 1*/
/* Fourni: Part 1, exercice 3, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 3, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant); //verifie que animal est bien seul
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y, float energie, Animal **liste_animal) {
	if ((x >= 0) && (x < SIZE_X) && (y >= 0) && (y < SIZE_Y)) {
		Animal* animal = creer_animal(x, y, energie);
		*liste_animal = ajouter_en_tete_animal(*liste_animal, animal);
		assert(liste_animal);
	}
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {	
	Animal *tmp = NULL; // element qui va etre enlevé
	Animal *pa = *liste; //pointeur qui parcours la liste
	while (pa && pa != animal){
		tmp = pa;
		pa = pa -> suivant;
	}
	if (pa){
		if(tmp){
			tmp -> suivant = pa -> suivant;
		}
		else{
			*liste = pa -> suivant; //cas ou l'animal enleve est le 1er
		}
	}
	free(pa);
}


/* A Faire. Part 1, exercice 5, question 2 */
Animal* liberer_liste_animaux(Animal *liste) {
    while (liste) {
        Animal* suivant = liste-> suivant;
        free(liste);
        liste = suivant;
    }
	return liste;
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la = la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 4, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa = NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j] = ' ';
      assert(ecosys[i][j]);
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    assert(ecosys[pa->x][pa->y]);
    pa = pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
        assert(ecosys[pa->x][pa->y]);
      } else {
        ecosys[pa->x][pa->y] = 'O';
        assert(ecosys[pa->x][pa->y]);
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");

  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
      assert(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie = compte_animal_it(liste_proie);
  int nbpred = compte_animal_rec(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}



/* PARTIE 2*/


void ecrire_ecosys(const char* nom_fichier, Animal* liste_predateur, Animal* liste_proie){
	FILE *f = fopen(nom_fichier,"w");
	if (f == NULL){
		printf("Erreur ouverture fichier\n");
		return;
	}
	printf("Ecriture de l'ecosystème...\n");
	fprintf(f,"<proies>\n");
	Animal* tmp = liste_proie;
	while(tmp){
		fprintf(f, "x = %d y = %d dir = [%d %d] e = %f \n",tmp -> x , tmp -> y , tmp -> dir[0] , tmp -> dir[1] ,tmp -> energie);
		tmp = tmp->suivant;
	}
	fprintf(f, "</proies>\n");
	fprintf(f, "<predateurs>\n");
	tmp = liste_predateur;
	while(tmp){
		fprintf(f, "x = %d y = %d dir = [%d %d] e = %f \n",tmp -> x , tmp -> y , tmp -> dir[0] , tmp -> dir[1] ,tmp -> energie);
		tmp = tmp->suivant;
	}
	fprintf(f, "</predateurs>\n");
	fclose(f);
	printf("Succès de l'écriture de l'ecosystème !\n");
}

void lire_ecosys(const char* nom_fichier, Animal** liste_predateur, Animal** liste_proie){
	FILE *f = fopen(nom_fichier,"r");
	if (f == NULL){
		printf("Erreur ouverture fichier\n");
		return;
	}
	printf("Lecture de l'ecosystème...\n");
	char buff[256];
	fgets(buff,256,f);
	assert(strncmp(buff,"<proies>",(long unsigned int) 8)==0);
	fgets(buff,256,f);
	int x , y , dir0 , dir1 ; 
	float e;
	while (strncmp(buff, "</proies>",(long unsigned int) 9) != 0){
		sscanf(buff, "x = %d y = %d dir = [%d %d] e = %f \n", &x , &y , &dir0 , &dir1 , &e);
		Animal* na = creer_animal(x,y,e);
		na -> dir[0] = dir0;
		na -> dir[1] = dir1;
		*liste_proie = ajouter_en_tete_animal(*liste_proie, na); // ou na ->suivant = *liste_proie; *liste_proie = na;
		fgets(buff,256,f);
	}
	fgets(buff,256,f);
	assert(strncmp(buff,"<predateurs>",(long unsigned int)12) ==0);
	fgets(buff,256,f);
	while(strncmp(buff, "</predateurs>",13) != 0){
		sscanf(buff, "x = %d y = %d dir = [%d %d] e = %f \n", &x , &y , &dir0 , &dir1 , &e);
		Animal* na = creer_animal(x,y,e);
		na -> dir[0] = dir0;
		na -> dir[1] = dir1;
		*liste_predateur = ajouter_en_tete_animal(*liste_predateur, na); // ou na ->suivant = *liste_proie; *liste_proie = na;
		fgets(buff,256,f);
	}
	fclose(f);
	printf("Succès de la lecture de l'ecosystème !\n");
}

/* Part 2. Exercice 4, question 1 */

void bouger_animaux(Animal *la) {
    Animal* tmp = la;
    while(tmp){
    	if((float) rand()/RAND_MAX	 < p_ch_dir){
    		tmp -> dir[0] = rand() % 3 -1;
    		tmp -> dir[1] = rand() % 3 -1;
    	}
    	tmp -> x = (tmp -> x - tmp -> dir[0] + SIZE_X) % SIZE_X;
    	tmp -> y = (tmp -> y - tmp -> dir[1] + SIZE_Y) % SIZE_Y;
    	tmp = tmp-> suivant;
    }

}

/* Part 2. Exercice 4, question 3 */

void reproduce(Animal **liste_animal, float p_reproduce) {
	Animal* ani = NULL;
	if (*liste_animal){
		ani = *liste_animal;
	}
	while (ani){
		if ((float) rand() / RAND_MAX < p_reproduce){
			ajouter_animal(ani->x, ani->y, ani->energie / 2 , liste_animal);
			ani->energie = ani->energie /2;
			
		}
		ani = ani -> suivant;
	}
}


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    if(*liste_proie){
    	bouger_animaux(*liste_proie);
    	Animal* tmp = *liste_proie;
    	while(tmp){
    		tmp -> energie --;
    		if (tmp -> energie <= 0.0){
    			enlever_animal(liste_proie, tmp);
    		} else{
    			if (monde[tmp->x][tmp->y] >= 0){
    				tmp -> energie = tmp-> energie + monde[tmp->x][tmp->y];
    			monde[tmp->x][tmp->y] = temps_repousse_herbe;
    			}
    		}
    		tmp = tmp -> suivant;
    	}
		reproduce(liste_proie, p_reproduce_proie);
	}
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
	Animal* tmp = l;
	while (tmp){
		if ((tmp -> x == x) && (tmp -> y == y)){
			return tmp;
		}
		tmp = tmp -> suivant;
	}	
	return tmp;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
	if(*liste_predateur){
    	bouger_animaux(*liste_predateur);
    	Animal* tmp = *liste_predateur;
    	while(tmp){
    		tmp -> energie --;
    		if (tmp -> energie <= 0.0){
    			enlever_animal(liste_predateur, tmp);
    		}
    		Animal* proie = animal_en_XY(*liste_proie, tmp -> x , tmp -> y);
    		if (proie){
    			tmp -> energie += proie -> energie;
    			enlever_animal(liste_proie, proie);
    		}
    		tmp = tmp -> suivant;
    	}
		reproduce(liste_predateur, p_reproduce_predateur);
	}
}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
	int i,j;
	for (i = 0 ; i < SIZE_X ; i++){
		for (j = 0 ; j < SIZE_Y ; j++){
			monde[i][j]++;
		}
	}
}

