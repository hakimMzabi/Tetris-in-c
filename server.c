#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#define BUFFERSIZE 1024 
#define MAXDELAY 3

int main(int argc, char **argv){
  
  int in,out;
  int PtoF[2];
  int FtoS;
  int pid1,pid2;
  char buffer[BUFFERSIZE];
  int on=1;
  int errno,nbytes;
  int status1,status2;
  int i;
  
  if(argc<3){
    printf("Usage: ./pipe inputfile outputfile\n");
    exit(1);
  }

  //Create an anonymous pipe and set it to be asynchronous
  pipe(PtoF);
  ioctl(PtoF[0],FIONBIO,&on);

  //Create a named pipe
  mkfifo("named_pipe",0666);

  //Fork Filter process
  if(fork()==0){ 
    //Open named pipe for writing
    FtoS=open("named_pipe",O_WRONLY);
    //Close unused write end of anonymous pipe
    close(PtoF[1]);
    //Constantly poll the read end of the anonymous pipe until EOF.
    //When data is read, convert it to lowercase and write it to named pipe
    while(1){
      sleep(rand()%MAXDELAY);
      nbytes=read(PtoF[0],buffer,BUFFERSIZE);
      if(nbytes>0){
        printf("Filter Read: %s",buffer);
        //Convert each char into a lowercase
        for(i=0;i<strlen(buffer);i++){
          buffer[i]=tolower(buffer[i]);
        }
        printf("Converted to: %s",buffer);
        write(FtoS,buffer,BUFFERSIZE);
      }
      if(nbytes==0){
        break;
      }
    }
    close(FtoS);
    close(PtoF[0]);
    printf("FINISHED FILTER.\n");
    exit(0);
  }

  //Fork Sink Process
  if(fork()==0){
    //Close unused anonymous pipe
    close(PtoF[0]);
    close(PtoF[1]);
    //Open named pipe for reading
    FtoS = open("named_pipe",O_RDONLY);
    out = open(argv[2],O_WRONLY);
    if(out==NULL){
      printf("Can't create output file\n");
      exit(1);
    }
    //Check if named pipe has data until EOF
    //When data is found, write into output.txt
    while(1){
      sleep(rand()%MAXDELAY);
      nbytes=read(FtoS,buffer,BUFFERSIZE);
      if(nbytes>0){
        printf("Sink read: %s",buffer);
        write(out,buffer,nbytes);
      }
      if(nbytes==0){
        break;
      }
    }
    close(out);
    close(FtoS);
    printf("FINISHED SINK.\n");
    exit(0);
  }

  //Parent Process
  //Close unused pipe ends
  close(PtoF[0]);
  in = open(argv[1],O_RDONLY);
  if(in==NULL){
    printf("Could not open input file.\n");
    exit(1);
  }
  //As long as there's something to read from the input.txt
  //Read it and write it to the anonymous pipe
  while(fgets(buffer,BUFFERSIZE,in)!=NULL){
    sleep(rand()%MAXDELAY);
    printf("Read from file: %s",buffer);
    write(PtoF[1],buffer,BUFFERSIZE);
  }
  printf("Finished Reading from file.\n");
  close(in);
  close(PtoF[1]);
  wait(&status1);
  wait(&status2);
  //named pipe cleanup
  unlink("named_pipe");
  printf("Finished everything!\n");
  
  return 0;
}