#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#ifndef BUFF_SIZE
#define BUFF_SIZE 8096
#endif


int main(int argc, char **argv) {

  int sockfd,len,request_sock,sstat;
  struct sockaddr_un server;
  char* buff;
  static struct timeval timeout = { 10, 0 };
  fd_set mask,rmask;


  if (argc < 2) {
    fprintf(stderr,"No Arguments Supplied\n");
    exit(1);
  }
  if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr,"ERROR: socket() error: ");
    exit(1);
  }
  server.sun_family = AF_UNIX;
  strncpy(server.sun_path,"/tmp/clamd", sizeof(server.sun_path));

  if(connect(sockfd,(struct sockaddr *)&server, sizeof(struct sockaddr_un)) == -1) {
    fprintf(stderr,"ERROR: can't connect(): \n");
    exit(1);
  }

  FD_ZERO(&mask); 
  FD_ZERO(&rmask); 
  FD_SET(sockfd, &mask); 
  FD_SET(fileno(stdin), &mask); 

  buff=(char*)malloc(BUFF_SIZE);
  strcpy(buff,argv[1]);
  if (argc == 3) {
    strcat(buff," ");
    strcat(buff,argv[2]);
  }

  len=write(sockfd,buff,strlen(buff));

  while (! FD_ISSET(sockfd,&rmask)) {
    rmask=mask;
    sstat=select(FD_SETSIZE, &rmask, (fd_set *)0, (fd_set *)0, &timeout);
    if (sstat == 0) {
      fprintf(stderr,"ERROR: select() timeout\n");
      free(buff);
      exit(1);
    }

   if (FD_ISSET(sockfd,&rmask)) {
      read(sockfd,buff,BUFF_SIZE);
      fprintf(stdout,"%s",buff);
    }

  }
  free(buff);
}
