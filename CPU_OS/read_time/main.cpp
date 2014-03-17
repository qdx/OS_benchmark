/* Output format:
 * reading time ie. getticks() overhead
 * elapsed() overhead
 * for() loop overhead
*/
#include <stdio.h>
#include <iostream>
#include "cycle.h"
using namespace std;

int main(int argc, const char* argv[]){
  
  // getticks() overhead:

  double sum = 0;
  for(int x = 0; x<10000; x++)
  {
    ticks t0 = getticks();
    ticks t1 = getticks();
    // double difference = elapsed(t1, t0);
    // cout << difference << endl;
    sum += elapsed(t1,t0);
  }
  double result = sum/10000.0;
  cout << result << endl;

  //elapsed() overhead

  double sum3 = 0;
  for(int x = 0; x<10000; x++)
  {
    ticks t4 = getticks();
    ticks t5 = getticks();

    ticks t6 = getticks();
    double d = elapsed(t4, t5);
    ticks t7 = getticks();

    sum3 += elapsed(t7, t6) - result;
  }

  cout  << sum3/10000.0 << endl;

  //for() loop overhead:

  double sum2 = 0;
  for(int x = 0; x<10000; x++)
  {
    int y = 0;
    ticks t2 = getticks();
    for(; y<10000; y++){}
    ticks t3 = getticks();
    sum2 += (elapsed(t3,t2) - result)/10000;
  }
  cout << sum2/10000.0 << endl;

}
