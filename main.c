#include "lib.h"
#include "deroulement.h"
#include "gerer_jeu.h"
#include "menu.h" 



int main (int argc,char **argv)
{
 if (argc < 2){
    
    system("clear");
    write(1,"*************************************************\n",sizeof("*************************************************\n"));
    write(1,"************* Bienvenue au termitris ************\n",sizeof("************* Bienvenue au termitris ************\n"));
    write(1,"*************************************************\n",sizeof("*************************************************\n"));
    bool test = true; 
    menu* m;
    menu* sm ,*sm1 ;
    write(1,"\t\t\t\tp : revenir\n",sizeof("\t\t\t\tp : revenir\n"));
   
    m = createMenu("\t*****$ Main menu $*****\n");
   
    
  //  Les Modes  
    
    sm = createMenu("Modes"); 
    addMenuAction(sm,"Facile",f);
    addMenuAction(sm,"Normal",f2);
    addMenuAction(sm,"Difficile",f3);
    addSubMenu(m,sm);

    //Regles du jeu

    addMenuAction(m,"Regles du jeu",f4);

    sm1 = createMenu("Multijoueurs");
    addSubMenu(m,sm1);
    addMenuAction(m,"Quitter",quit);
    
   // quitter
    
  
   while(test) launchMenu(m);
  
  deleteMenu(m);
  }

 
 return EXIT_SUCCESS;
}
