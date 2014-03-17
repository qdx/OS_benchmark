#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "cycle.h"
#include "network.h"

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    int portno;
    portno = atoi(argv[2]);

    int my_socket;
    my_socket = create_new_socket();
    if (my_socket < 0) error("ERROR opening socket");

    struct sockaddr_in serv_addr;
    sockaddr_generator(&serv_addr, argv[1], portno);
    struct sockaddr* serv_sockaddr = (struct sockaddr *) &serv_addr;

    if (connect(my_socket, serv_sockaddr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    char* buffer = "a";

    char read_buffer[256];
    bzero(read_buffer,256);

    int kk = 0;
    int write_n, read_n;
    ticks t1, t2;
    double sum = 0.0;
    int loop_times = 1000;
    /*printf("before testing!\n");*/
    for(kk = 0 ; kk < loop_times; kk ++){
      bzero(read_buffer,256);
      /*printf("I write:%s\n", buffer);*/
      read_n = read(my_socket,read_buffer,255);
      t1 = getticks();
      write_n = write(my_socket,buffer,strlen(buffer));
      read_n = read(my_socket,read_buffer,255);
      t2 = getticks();
      double etime = elapsed(t2,t1);
      sum += etime;
      /*printf("elapsed time: %f milliseconds\n", etime * 0.417 / 1e6);*/
      if (write_n < 0) 
        error("ERROR writing to socket");
      if (read_n < 0) 
        error("ERROR reading from socket");
      write_n = write(my_socket,buffer,strlen(buffer));
      /*printf("I read: %s\n", read_buffer);*/
    }

    printf("%f\n", (sum / loop_times) * 0.417 / 1e6);
    close(my_socket);
    return 0;
}
