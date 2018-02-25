#include <stdio.h>
#include "lib.h"
#include "deroulement.h"
#include "gerer_jeu.h"
#include "menu.h"


/*la fonction pour les la carte du mode à partir d'un fichier*/

char* lire_carte(char* path){
	char* debut = malloc(BUFF_SIZE * sizeof(char));
	int fd = open(path,O_RDONLY);
	if(fd < 0) { perror("fd");}
	ssize_t nread=read(fd,debut,BUFF_SIZE);
	if(nread < 0) { perror("nread"); }
	close(fd);
	return debut;
}

/*la fonction pour recupere la taille de la base sur la quelle les figures vont tombées*/
char** get(char** debut,int* h,int* l){
	
	char* pointeur = strpbrk(*debut,"\n");/*depasser les \n*/
	if(pointeur!=NULL){
		char* espace =strpbrk(*debut," ");/*depasser les espaces*/
		if(espace!=NULL){
			*h=atoi(*debut);      //transformation de la dimention de la base de char vers int  /
			*l=atoi(++espace); 
		}
	}
	*debut = ++pointeur; 
	char** tab= malloc( (*h) * sizeof(char*));
	if(tab==NULL){perror("malloc");}
	for(int i=0;i<(*h);i++){// pour chaque ligne du tab on va allouer le nb de colonne
		tab[i] = malloc( (*l) * sizeof(char));
		if(tab[i]==NULL){perror("malloc");}
		for(int j=0;j<(*l);j++){
			if((*debut)[0]!='\n'){
				
				//pour remplacer les zero par '0'

				if((*debut)[0]=='1'){
					tab[i][j] = '1';
				}else if((*debut)[0]=='0'){//remplissage de la base 
					tab[i][j] ='0';
				}else{
					tab[i][j] = (*debut)[0];					
				}
				(*debut)++;
			}
		}
		(*debut)++;
	}
	return tab;
}

/*la fonction pour recupere le nombre de briques qui existe dans le mode */

int get_nbr(char** debut){
	int nbr_briques = atoi(*debut);
	*debut+=2;
	return nbr_briques;
}

/* la fonction pour recupere la vitesse du mode*/

float get_vitesse(char** debut){
	float v = atof(*debut);
	*debut+=2;
	return v;
}

/*la fonction pour remplire le tab de struct brique à partir du fichier mode*/

struct_b* get_tab_briques(char** debut,int nbr_briques){
	struct_b * brique = malloc(nbr_briques * sizeof(struct_b));
	if(brique==NULL){perror("malloc");}
	  for(int i = 0 ; i < nbr_briques ; i++){
		brique[i].tab = get(debut,&brique[i].h,&brique[i].l);
		
	}
	return brique;
}

/*la fonction pour lire et recuperer le nombre max des figures qui vont tombéss*/

int* get_tab(char** debut , int max_briques){ 
	int * t = malloc(max_briques * sizeof(int));
	if(t==NULL){perror("malloc");}
	for(int i = 0 ; i < max_briques ; i++){
		t[i] = atoi(*debut);
		*debut+=2;	
	}
	return t;
}


int NBR_RETOUR_LIGNE = 0;/*une variable globale pour la fonction retour_ligne*/

/*la fonction pour faire le retour à la ligne apres chaque affichage*/

void retour_ligne(int h){
	char* retour_line = malloc( h * sizeof(char) );
	// deplacer le curseur a la position (i+1,0) pour faire un retour ligne '\n'
	int nbr = sprintf(retour_line, "\x1b[%d;%dH", NBR_RETOUR_LIGNE+1, 0);
	write(STDOUT_FILENO,retour_line,nbr);// afficher '\n'
	NBR_RETOUR_LIGNE++;
}
    

/*la fonction pour affichier la carte du jeu*/

void afficher_tab(char** tab, int h, int l ){
	
  
	NBR_RETOUR_LIGNE = 0;
	retour_ligne(h);
	char* buf = malloc(sizeof(char));
	int n ;
	for(int ii=0;ii<h;ii++){
		for(int jj=0;jj<l;jj++){
			if(tab[ii][jj]=='1'){

				write(1,"\x1b[92m",sizeof("\x1b[92m"));
				write(1,"▣",strlen("▣") );
				
			}
			else if( tab[ii][jj]=='0'){
				
				write(1," ",strlen(" ") );
				
			}
			else{
				n =sprintf(buf, "\x1b[37m%c", tab[ii][jj]);
				write(STDOUT_FILENO,buf,n); //ecrire(dans l'ecran ) un char a partir de carte[][] 
			}
		}
		retour_ligne(h);
    }	
}




/*une fonction pour etablir le mode raw*/

int raw_mode(struct termios* canon){
	// on stoke les parametres par default dans (canon) pour les reutiliser quand on vaut revinir en mode canonique
	tcgetattr(STDIN_FILENO,canon);
	//raw mode
	struct termios raw;
	// on stoke les parametres par default dans (raw) 
	tcgetattr(STDIN_FILENO,&raw); 
	//cfmakeraw modifie les flags pour le mode raw (mode non canonique)
	cfmakeraw(&raw);  
	//sauvgarder les modification
	tcsetattr(STDIN_FILENO,TCSANOW,&raw);

	return EXIT_SUCCESS;
}

/*une fonction pour retablir le canonique*/

int canonique_mode(struct termios* canon){
	//sauvgarder les parametre par default (l'etat avant la modification de cfmakraw)
	tcsetattr(STDIN_FILENO,TCSANOW,canon);
	return EXIT_SUCCESS;
}

/*la fonction pour affichier le jeu à partir des tableau de fig carte*/

void remplir_carte(struct_c* carte,char** premieres_lignes,int hauteur){
	carte->tab = malloc(carte->h * sizeof(char*));
	if(carte->tab==NULL) {perror("malloc");}
	for(int i = 0 ; i < carte->h ; i++){
		carte->tab[i] = malloc( carte->l * sizeof(char));
		if(carte->tab[i]==NULL){perror("malloc");}
	}
	int pos = (carte->h - hauteur); 
	for(int k = 0 ; k < carte->h ; k++){
		for(int j = 0 ; j < carte->l ; j++){
			if(k < pos){carte->tab[k][j]='0';}
			else{ carte->tab[k][j] = premieres_lignes[k-pos][j];}
		}
	}
}

/*la fonction pour deposer une brique dans la carte à partir d'une position*/

void set_brique(struct_c* carte , struct_p* p , struct_b* brique , int b){
	for(int i = 0 ; i < brique[b].h ; i++){
		for(int j = 0 ; j < brique[b].l ; j++){
			if(brique[b].tab[i][j]=='1'){
				carte->tab[p->x+i][p->y+j]= brique[b].tab[i][j] ;
			}
		}
	}
}

/*une fonction pour supprimer la figure d'une ancienne position*/

void rm_brique(struct_c* carte , struct_p* p , struct_b* brique , int b){
	for(int i = 0 ; i < brique[b].h ; i++){
		for(int j = 0 ; j < brique[b].l ; j++){
			carte->tab[p->x+i][p->y+j]= '0' ;
		}
	}
}

/*la fonction qui gere le mouvement des briques*/

int move(struct_c* carte, struct_p* p , struct_b* brique ,int b , char c ){
 
  if( c == 'B'){  // down
    if(p->x+brique[b].h < carte->h ){
      for (int i = 0; i < brique[b].l ; i++){
        if((carte->tab[p->x+brique[b].h][p->y+i]=='1')&&(carte->tab[p->x+brique[b].h-1][p->y+i]=='1')){
          return 1;
        }
      }
      rm_brique(carte,p,brique,b);
      p->x++;
    }else{ 
      set_brique(carte,p,brique,b);
      return 1;
    }
    set_brique(carte,p,brique,b);
  }
  else if( c == 'C'){  // Droite
    if(p->y+brique[b].l < carte->l){
      for (int i = 0; i < brique[b].h ; i++){
        if((carte->tab[p->x+i][p->y+brique[b].l-1]=='1')&&(carte->tab[p->x+i][p->y+brique[b].l]=='1')){
          return 0;
        }
      }
      rm_brique(carte,p,brique,b);
      p->y++;
    }
    set_brique(carte,p,brique,b);
  }
  else if( c == 'D'){  // Gauche
    if(p->y-1 >=0){
      for (int i = 0; i < brique[b].h ; i++){
        if((carte->tab[p->x+i][p->y-1]=='1')&&(carte->tab[p->x+i][p->y]=='1')){
          return 0;
        }
      }
      rm_brique(carte,p,brique,b);
      p->y--;
    }
    set_brique(carte,p,brique,b);
  }
  return 0;
}

bool lost_test (struct_c* carte){
   
   bool lost = false;
   for (int i = 0; i < carte->l; i++)
   {
   	if(carte->tab[0][i] =='1')
   		{
   		  lost = true;	
   		}
   }


  return lost;
}



void poll_fonction(struct_c* carte, struct_p* p ,struct_b* brique , int* tab ,int max_briques,float vitesse ,struct termios* canon){
	
	int rv,alt = 0;
	p->x = 0;
	p->y = carte->l/2;	
	struct pollfd pfds[1]; 
	char buf[3]; //buf[3] = {'\e','[','ABCD'}
	int i = 0;
	int b = tab[i]; // tab[i] -1
	pfds[0].fd = STDIN_FILENO; 
	pfds[0].events = POLLIN;
    int timeout = vitesse*1000;
	 write(1,"\x1b[?25l",sizeof("\x1b[?25l"));  
	  while(i<max_briques){ 
	  	/*if (lost_test(carte)){
	    		write(1,"\n\t\tvous avez perdu :(\n",strlen("\n\t\tvous avez perdu :(\n"));
	    		canonique_mode(canon);
	    		exit(1);
	    	}*/
	        rv = poll(pfds, 1,timeout);
	   		if (rv == -1){  perror("poll"); }   
	    		 else if (rv == 0){    
	    			if(move(carte,p,brique,b,'B')){
	    		
	        			i++;
	        			b = tab[i];
	        			p->x = 0;
	        			p->y = carte->l/2;
	        	
	        		}      
	    	afficher_tab(carte->tab,carte->h,carte->l);

	    	
	    	
	        } 
	        else if (rv > 0) {// en cas ou poll a recu qlq chose 
			if(pfds[0].revents & POLLIN) {//si l'entree standard a lu qq chose
				ssize_t nb_lu =read(STDIN_FILENO,&buf,3); 
				if(nb_lu == -1) { perror("read4");exit(1);}
				if (nb_lu >0){ 
					if(buf[0] == '\e') {
						if(move(carte,p,brique,b,buf[2])){
							i++;
							b = tab[i];// tab[i] -1
							p->x = 0;
							p->y = carte->l/2;
							alt = alt+carte->h+brique[i].h;
						}
					}

					if(buf[0] == 'q') {
						write(1,"\x1b[?25h",sizeof("\x1b[?25h"));  
						canonique_mode(canon); //revenir en mode canonique
						exit(1);
					} 
				}
        	}
  		     
  		}
 	}
}

ssize_t full_write(int fdesc, const char* buff, size_t towrite){
  int nwrite, length = 0;
  
  while(length < towrite){ /* On lit tant qu'il y a des choses a lire dans le fichier */
    nwrite = write(fdesc, buff + length, towrite - length);  /* On écrit dans le descripteur fdesc le contenu du buffer buff */             
    if(nwrite == -1)return -1;
    length += nwrite; /* On incrémente lenght au fur et a mesure avec tous ce qui est écrit dans nwrite  */
  }
  return length;
}


int contenu_fichier(char file[]){
  int fdesc;
  ssize_t nread;
  char buff[1024];
  
  fdesc = open(file, O_RDONLY); /* On ouvre le fichier dans un descripteur fdesc */
  if(fdesc == -1){
  	perror(file); 
  	return -1;
  }
  while((nread = read(fdesc, buff,sizeof(buff)))){  /* On boucle tant qu'il y a des choses a lire */     
    if(write(STDOUT_FILENO, buff, nread) == -1){perror(file); return -1;} /* On appel la fonction full_write */     
  }

  close(fdesc); /* On ferme le descripteur de fichier */ 
  return 0;
}









