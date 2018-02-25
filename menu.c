#include "lib.h"
#include "deroulement.h"
#include "menu.h"
#include "gerer_jeu.h"

//creation d'un menu avec 5 partitions non initialisé

menu *createMenu(const char* text)
{
  int i;
  int t = strlen(text)+1;
  menu * M1 = malloc (sizeof(menu));
  M1->parent = NULL;
  M1->desc = malloc (t*sizeof(char));
  strcpy(M1->desc,text);
  for(i=0;i<5;i++)
   {
     M1->items[i].it_flag = NI;
   }
  
  return M1;
  
}
//ajouter une action a un menu

void addMenuAction(menu* m, const char* text, void(*f)(const char* s)) 
{
  int i ;
  int t = strlen(text)+1;
  
  for (i=0;i<5;i++)
  {
    if ( m->items[i].it_flag == NI)
    {
       break;
    }
   }  
      m->items[i].it_flag = ACT;
      m->items[i].it_content.action.desc_a = malloc(t*sizeof(char));  
      strcpy(m->items[i].it_content.action.desc_a,text);
      m->items[i].it_content.action.f = f;   
}


// ajouter un sous-menu a un menu 

void addSubMenu(menu* m, menu* sm)
{
  int i;
  if (sm->parent != NULL){exit(EXIT_FAILURE); }

  for (i=0;i<5;i++)
  {
    if ( m->items[i].it_flag == NI)
  	{
         break;
	}
    }
	
        m->items[i].it_flag = SM;
  	    m->items[i].it_content.sous_menu = sm ;
  	    sm->parent= m;
 
}

//suppression du menu

void deleteMenu(menu* m)
{
  int i;
  if (m->parent == NULL)
  {
     for (i=0;i<5;i++)
    {
      free(m->items[i].it_content.sous_menu); 
    }
  }
 free(m);

}

//lancer le menu

void launchMenu(menu* m) 
{
  char rep;
  
  int i;
  char buf[20];
 int n = sprintf(buf,"%s\n",m->desc);
 write(1,buf,n);  
   
  for(i=0;i<5;i++)
    {
      if(m->items[i].it_flag == SM)
       {
          printf("%d-%s\n",i+1,m->items[i].it_content.sous_menu->desc);
       }
  	 
       if(m->items[i].it_flag == ACT)
       {
          printf("%d-%s\n",i+1,m->items[i].it_content.action.desc_a);
       }
   }
   
  write(1,"\nchoise?:\n",10); 
   

     do {rep = getchar(); } while ( (rep == '\n') || (rep == '\r') );
      
        
  switch (rep)
  {
    case '1' : system("clear");
               
               if (m->items[0].it_flag == ACT){m->items[0].it_content.action.f("./facile");break;}

               if (m->items[0].it_flag == SM){launchMenu(m->items[0].it_content.sous_menu);}
 	             break;

    case '2' : system("clear");
               if (m->items[1].it_flag == SM){launchMenu(m->items[1].it_content.sous_menu);}
               if (m->items[1].it_flag == ACT){m->items[1].it_content.action.f("./normal");break;}
 		           break;
    
    case '3' : system("clear");
               if (m->items[2].it_flag == ACT){m->items[2].it_content.action.f("./difficile");break;}
               if (m->items[2].it_flag == SM){launchMenu(m->items[2].it_content.sous_menu);}
               break;
    
    case '4' : if (m->items[3].it_flag == ACT){m->items[3].it_content.action.f(" ");break;}
               if (m->items[3].it_flag == SM){launchMenu(m->items[3].it_content.sous_menu);}
 		break;
    
    case '5' : if (m->items[4].it_flag == ACT){m->items[4].it_content.action.f(" ");break;}
               if (m->items[4].it_flag == SM){launchMenu(m->items[4].it_content.sous_menu);}
 		break;
    
    
    case 'p' :  system("clear");
                if (m->parent !=NULL)  { launchMenu (m->parent);}
                break;
  
     default : break;//printf ("verifiez la commande SVP Oo !!\n");
       
 }
}

void f(const char* chemin){//mode facile
 
   game(chemin);
  }

void f2(const char* chemin){//mode normal

  game(chemin);

}


void f3(const char* chemin){//mode difficle

  game(chemin);

}

void f4(){//lire le fichier rules


  int fdesc;
  ssize_t nread;
  char buff[1024];
  
  fdesc = open("Termtris_rules.txt", O_RDONLY); /* On ouvre le fichier dans un descripteur fdesc */
  if(fdesc == -1){
    perror("open"); 
  }
  while((nread = read(fdesc, buff,sizeof(buff)))){  /* On boucle tant qu'il y a des choses a lire */     
    if(write(STDOUT_FILENO, buff, nread) == -1){perror("write");} /* On appel la fonction full_write */     
  }

  close(fdesc); /* On ferme le descripteur de fichier */ 

}


bool test = true;

void quit() {

system("clear");  
printf("Merci et au revoir ^_^ \n"); 
 exit(0);
}
