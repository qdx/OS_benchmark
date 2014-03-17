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
  listen(my_socket,100);

  printf("===tcp rount trip time experiment===\n");
  while(1){
    printf("Log: waiting for client to connect!\n");
    int newsockfd;
    // accept will block until some client connect to the port
    newsockfd = accept_incoming_connection(my_socket);
    printf("Log: client accepted!\n");
    if (newsockfd < 0) 
      error("ERROR: error when accepting connection");

    int read_n, write_n, kk;
    char buffer[256];
    char* ack = "a";
    int loop_times = 1000;
    printf("Log: testing round trip 1000 times!\n");
    for( kk = 0 ; kk < loop_times ; kk ++ ){
      // read and print the message sent from client
      bzero(buffer,256);
      /*printf("\n++debug++ before read\n");*/
      // read should block too, not sure ??
      write_n = write(newsockfd, ack, strlen(ack));
      read_n = read(newsockfd, buffer, 255);
      /*printf("Here is the message: %s\r",buffer);*/

      // write ack back to client
      /*printf("\n++debug++ before write\n");*/
      write_n = write(newsockfd, ack, strlen(ack));
      read_n = read(newsockfd, buffer, 255);
      if (read_n < 0){
        printf("reading fault\n");
        error("ERROR: reading from socket failed");
      }
      if (write_n <= 0){
        printf("writing fault\n");
        error("ERROR: writing to socket failed");
      }
      /*printf("\n++debug++ end of while\n");*/
    }
    // close connection
    close(newsockfd);
    printf("Log: client left!\n\n");
  }

  close(my_socket);
  return 0; 
}
