#include <stdio.h>
#include <iostream>
#include "cycle.h"

using namespace std;

double measure_fork_overhead(int max_iteration){
  double ticks_sum = 0.0;
  for(int i = 0 ; i < max_iteration; i++){
    ticks t1 = getticks();
    if(fork() == 0)
      _exit(0);
    ticks t2 = getticks();
    ticks_sum += elapsed(t2, t1);
  }
  return ticks_sum/(double)max_iteration;
}

int main(int argc, const char* argv[]){
  cout << measure_fork_overhead(10000) << endl;
}
