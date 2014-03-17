#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include "network.h"

/* A simple server  using TCP */
void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr,"ERROR: no port provided\n");
    exit(1);
  }

  // create a new socket
  int my_socket;
  my_socket = create_new_socket();
  if (my_socket < 0) 
    error("ERROR: failed to create new socket");

  int portno;
  portno = atoi(argv[1]);

  struct sockaddr_in serv_addr;
  sockaddr_generator(&serv_addr, NULL, portno);

  //bind
  if (bind(my_socket, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
    error("ERROR: binding to socket failed");

  // listen: on my_socket with queue size 5
  // TODO try differnt queue size here
  /*listen(my_socket,10);*/
  listen(my_socket,30);

  printf("===tcp rount trip time experiment===\n");
  while(1){
    printf("Log: waiting for client to connect!\n");
    int newsockfd;
    newsockfd = accept_incoming_connection(my_socket);
    printf("Log: client accepted!\n");
    if (newsockfd < 0) 
      error("ERROR: error when accepting connection");

    int read_n, write_n;
    // TODO try differnt packet size for local bandwidth
    char buffer[257];
    char* ack = "a";
    int loop_times = 4*1024*2000;
    printf("Log: testing bandwidth times!\n");
    while(1){
      bzero(buffer,257);
      read_n = read(newsockfd, buffer, 256);
      /*printf("Here is the message: %s\r",buffer);*/
      if (read_n <= 0) break;
    }
    // close connection
    close(newsockfd);
    printf("Log: client left!\n\n");
  }

  close(my_socket);
  return 0; 
}
