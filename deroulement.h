#ifndef DEROULEMENT_H
#define DEROULEMENT_H
#include <stdio.h>

/*structure pour les posissions*/

typedef struct point{
	int x; 
	int y;
}struct_p;

/*structure pour la carte du jeu*/

typedef struct carte{
	int h;
	int l;
	char** tab;
}struct_c;

/*structure pour les briques "figures"*/
typedef struct struct_b {
	int h;
	int l;
	char** tab;
}struct_b;


char* lire_carte(char* path);

char** get(char** debut,int* h,int* l);

int get_nbr(char** debut);

float get_vitesse(char** debut);

struct_b* get_tab_briques(char** debut,int nbr_briques);;

int* get_tab(char** debut , int max_briques);

void retour_ligne(int h);

void afficher_tab(char** tab, int h, int l );

int raw_mode(struct termios* canon);

int canonique_mode(struct termios* canon);

void remplir_carte(struct_c* carte,char** premieres_lignes,int hauteur);

void set_brique(struct_c* carte , struct_p* p , struct_b* brique , int b);

void rm_brique(struct_c* carte , struct_p* p , struct_b* brique , int b);

int move(struct_c* carte, struct_p* p , struct_b* brique ,int b , char c );

bool tst (struct_c* carte);

void poll_fonction(struct_c* carte, struct_p* p ,struct_b* brique , int* tab ,int max_briques,float vitesse ,struct termios* canon);


#endif