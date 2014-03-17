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

    // a 256B buffer
    // TODO try differnt packet size for local bandwidth
    char buffer[257];
    bzero(buffer, 257);
    int kk;
    for(kk = 0 ; kk < 257; kk ++){
      // initialize buffer
      buffer[kk] = 'x';
    }

    int write_n, read_n;
    ticks t1, t2;
    double sum = 0.0;
    // write 10MB to socket
    int loop_times = 4*1024*2000;
    t1 = getticks();
    for(kk = 0 ; kk < loop_times; kk ++){
      write_n = write(my_socket,buffer,strlen(buffer));
      if (write_n < 0) 
        error("ERROR writing to socket");
    }
    /*printf("%d\n",kk);*/
    /*bzero(buffer, 257);*/
    /*read_n = read(my_socket, buffer, 256);*/
      /*write_n = write(my_socket,buffer,strlen(buffer));*/
    t2 = getticks();
    double etime = elapsed(t2,t1);

    printf("%f\n", (loop_times/(1024*4)) / (etime * 0.417 / 1e9));
    close(my_socket);
    return 0;
}
