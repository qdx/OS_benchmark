#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include "network.h"
#include "cycle.h"

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

  printf("===tcp setup time experiment===\n");
  while(1){
    int kk;
    int loop_times = 100;
    int newsockfd;
    char* ack = "a";
    char buffer[256];
    int read_n, write_n;
    printf("ready and wait to run tcp setup and tear down for %d times.\n", loop_times);
    double sum = 0.0;
    double sum2 = 0.0;
    double sum3 = 0.0;
    for( kk = 0 ; kk < loop_times; kk ++ ){
      bzero(buffer,256);
      // accept will block until some client connect to the port
      newsockfd = accept_incoming_connection(my_socket);
      /*if (newsockfd < 0) */
        /*error("ERROR: error when accepting connection");*/

      // read and print the message sent from client
      // read should block too, not sure ??
      /*read_n = read(newsockfd, buffer, 255);*/
      // write ack back to client
      /*write_n = write(newsockfd, ack, strlen(ack));*/
      /*ticks t0 = getticks();*/
      read_n = read(newsockfd, buffer, 255);

      ticks t1 = getticks();
      /*if (read_n < 0) error("ERROR: reading from socket failed");*/
      /*if (write_n <= 0) error("ERROR: writing to socket failed");*/
      close(newsockfd);
      ticks t2 = getticks();
      double etime = elapsed(t2, t1);
      /*double etime2 = elapsed(t1, t0);*/
      /*double etime3 = elapsed(t2, t0);*/
      sum += etime;
      /*sum2 += etime2;*/
      /*sum3 += etime3;*/
    }
    printf("%f\n", (sum / loop_times) * 0.417 / 1e6);
    /*printf("%f\n", (sum2 / loop_times) * 0.417 / 1e6);*/
    /*printf("%f\n", (sum3 / loop_times) * 0.417 / 1e6);*/
    // close connection
    printf("Log: client left!\n\n");
  }

  close(my_socket);
  return 0; 
}
