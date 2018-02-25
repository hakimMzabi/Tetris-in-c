#include <stdio.h>
#include "lib.h"


int main (int argc,char** argv)
{
   
   int lu,lu1 ;
   char str[5000];
   char buff [2048];
   
   char* fifo2 = "/tmp/client";
   char* fifo1 = "/tmp/serveur"; 
     
   mkfifo(fifo2,0666);

   int entree_tube = open(fifo1,O_RDONLY );
   int sortie_tube;// = open(fifo2,O_WRONLY);
   while(1){
    
   lu =  read(entree_tube, str, sizeof(str));
   
   if(lu>0)
   {
      system("clear");
      write(1, str, lu);}
   }
   
   lu1 = read(0,buff,sizeof(buff));
   write(sortie_tube,buff,lu1);

  
    
return 0;

}