/************************ GERER_JEU ****************************/
#include <stdio.h>
#include "lib.h"
#include "gerer_jeu.h"
#include "deroulement.h"


/*
 * Fonction game : lance le jeu avec la carte choisi
 * Arguments (paramètres) : 
 * chemin : arborescence du chemin d'acces au carte
 */

int game(const char * chemin){
 
  DIR * rep_mod;
  printf("%s\n",chemin );
  int hauteur , largeur ;
  struct termios canon;
  chdir("mod");
  rep_mod = opendir(chemin);
  struct dirent* arbo;
  chdir(chemin);
  while((arbo = readdir(rep_mod))){
    system("reset"); /* On clear le terminal pour afficher la carte du jeu */
    raw_mode(&canon); /* On passe en mode non-canonique */
    
    if(strcmp(arbo->d_name,".") == 0 || strcmp(arbo->d_name,"..") == 0){ /* On évite les repertoires "." et ".." */
      continue;
    }

  
  char* debut = lire_carte(arbo->d_name);
  char** premieres_lignes = get(&debut,&hauteur,&largeur);
  int nbr_briques  = get_nbr(&debut);
  struct_b* brique = get_tab_briques(&debut,nbr_briques); 
  float vitesse_a    = get_vitesse(&debut); 
  float vitesse_b    = get_vitesse(&debut);
  float vitesse      = vitesse_a/vitesse_b;
  int max_briques  = get_nbr(&debut);
  int* tab         = get_tab(&debut,max_briques);
  struct_p p;
  struct_c carte;
  carte.h = 20 ; // a changer apres
  carte.l = largeur ;
  remplir_carte(&carte,premieres_lignes, hauteur);
     
  poll_fonction(&carte , &p , brique ,tab , max_briques , vitesse , &canon);    

  }
  canonique_mode(&canon); /* On passe en mode canonique */
  write(1,"félecitation Niveau terminé ^_^\n\n",strlen("félecitation Niveau terminé ^_^\n\n"));
  closedir(rep_mod);
  chdir("./.."); /* on se remet dans le dossier mode */
  return 0;     
 
}