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


    struct sockaddr_in serv_addr;
    sockaddr_generator(&serv_addr, argv[1], portno);
    struct sockaddr* serv_sockaddr = (struct sockaddr *) &serv_addr;

    int connect_result;
    int i;
    int loop_times = 100;
    double sum_setup = 0.0;
    double sum_teardown = 0.0;
    for(i = 0 ; i < loop_times ; i ++){
      int my_socket;
      my_socket = create_new_socket();
      if (my_socket < 0) error("ERROR opening socket");
      ticks t1 = getticks();
      connect_result = connect(my_socket, serv_sockaddr, sizeof(serv_addr));
      ticks t2 = getticks();
      if(connect_result > 0 )
        printf("connect success\n");
      double etime = elapsed(t2, t1);
      sum_setup += etime;

      /*char buffer[256];*/
      /*bzero(buffer,256);*/
      /*char* ack = "a";*/
      /*write(my_socket, ack, strlen(ack));*/
      /*read(my_socket, buffer, 255);*/

      t1 = getticks();
      close(my_socket);
      t2 = getticks();
      etime = elapsed(t2, t1);
      /*printf("%f\n", (etime) * 0.417 / 1e6);*/
      sum_teardown += etime;
    }
    printf("%f\n", (sum_setup / loop_times) * 0.417 / 1e6);
    printf("%f\n", (sum_teardown / loop_times) * 0.417 / 1e6);
    return 0;
}
