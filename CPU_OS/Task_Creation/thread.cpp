#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cycle.h"
#include <iostream>

using namespace std;


void *thread_fn( void *ptr ){};

double measure_kernel_thread(int iteration_times){
  pthread_t thread ;
  const char *message = "Thread ";
  int  iret;

  double sum_ticks = 0.0;
  for(int i = 0 ; i < iteration_times ; i ++){
    ticks t1 = getticks();
    iret = pthread_create( &thread, NULL, thread_fn, (void*) message);
    ticks t2 = getticks();
    sum_ticks += elapsed(t2, t1 );
  }
  return sum_ticks/(double)iteration_times;
}

main() {
  cout << measure_kernel_thread(10000) << endl;
  exit(0);
}



