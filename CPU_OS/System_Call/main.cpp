/* Output format:
 * system call overhead for getppid()
*/

#include <stdio.h>
#include <iostream>
#include "cycle.h"
#include <unistd.h>
#include "sys/syscall.h"

using namespace std;

int main(int argc, const char* argv[]){
  
  int iterations = 10000;
  int pid;
  double tick_overhead = 45.0;
  

  double sum = 0.0;
  for(int x = 0; x<iterations; x++)
  {
    ticks t1 = getticks();
    getppid();
    ticks t2 = getticks();
    sum += (elapsed(t2,t1) - tick_overhead);
  }

  cout << sum/iterations << endl;
}
