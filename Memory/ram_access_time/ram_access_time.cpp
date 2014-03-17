#include <stdio.h>
#include <iostream>
#include "cycle.h"
#include <math.h>
#include <stdlib.h> 
using namespace std;

// #define ARRAYSTRIDE 32 //change this each time accordingly
#define ITERATIONS 1000000


void measure_access_time(int arraysize, int arraystride)
{

  //CREATE ARRAY WITH STRIDE
  int* arr = new int[arraysize];
  for (int x = 0; x<arraysize; x++)
  {
    arr[x] = -1;
  }

  int fillIterations = arraysize/arraystride;
  // cout << fillIterations << endl;
  int range = arraysize/(arraystride);

  int p = 0;
  int f = 0;
  //FILL ARRAY ACCORDING TO STRIDE
  for(f = arraystride; f < arraysize; f+=arraystride)
  {
    arr[p] = f;
    p = f;
  }
  arr[p] = 0;
  arr[arraysize-1]=0;


  /*
  * debug - print out all elements of array
  */
  // for (int x = 0; x<arraysize; x++)
  // {
  //   cout << arr[x] << endl;
  // }

  //CALCULATE
  double sum = 0.0;
  int clicks = 0;
  int c = 0; //NOTE: change this back to random number generator when benchmarking
  int access = 0;

  ticks t1 = getticks();
  for(int i = 0; i<ITERATIONS; i++)
  {
    clicks++; //part of old code, too lazy to change
    c = access;
  }
  ticks t2 = getticks();
  double overhead = elapsed(t2,t1) - 45.0;

  clicks = 0;
  ticks t3 = getticks();
  for(int i = 0; i<ITERATIONS; i++)
  {
    clicks++; //part of old code, too lazy to change
    access = arr[c];
    c = access;
  }
  ticks t4 = getticks();
  sum += elapsed(t4,t3) - 45.0;

  //this is time in ns:
  // cout << arraysize << endl; 
  delete[] arr;
  cout << ((sum-overhead)/clicks) * 0.416 << endl; 

}



int main(int argc, const char* argv[])
{
  //8192 (2^15) = 32KB -> L1 cache
  //786432 (2^21.6)= 3072KB -> L2 cache
  //testing: 2^10 (1KB) -> 2^23 (?MB)    22->4MB

  int minsize = (int) pow(2,10);
  long long int maxsize = (long long int) pow(2,30);
  int stridesize;


  /*
  * prints out the size of the memory
  */
  // cout << "<Memory sizes: >" << endl;
  // for(long long int memsize = minsize; memsize <=maxsize; memsize=memsize*2 )
  // {
  //   cout << memsize << endl;
  // }

  /*
  * test v.1
  */
  // stridesize = (int) pow(2,5); //128B
  // cout << "<Stride = >" << stridesize << endl;
  // for(long long int memsize = minsize; memsize <=maxsize; memsize=memsize*2 )
  // {
  //   int testing_size = memsize/4;
  //   measure_access_time(testing_size, stridesize);
  // }

  // stridesize = (int) pow(2,13); //8KB
  // // cout << "<Stride = >" << stridesize << endl;
  // for(long long int memsize = minsize; memsize <=maxsize; memsize=memsize*2 )
  // {
  //   int testing_size = memsize/4;
  //   measure_access_time(testing_size, stridesize);
  // }

  // stridesize = (int) pow(2,14); //16KB
  // // cout << "<Stride = >" << stridesize << endl;
  // for(long long int memsize = minsize; memsize <=maxsize; memsize=memsize*2 )
  // {
  //   int testing_size = memsize/4;
  //   measure_access_time(testing_size, stridesize);
  // }

  // stridesize = (int) pow(2,15); //32KB
  // // cout << "<Stride = >" << stridesize << endl;
  // for(long long int memsize = minsize; memsize <=maxsize; memsize=memsize*2 )
  // {
  //   int testing_size = memsize/4;
  //   measure_access_time(testing_size, stridesize);
  // }

  // stridesize = (int) pow(2,20);
  // cout << "<Stride = >" << stridesize << endl;
  // for(long long int memsize = minsize; memsize <=maxsize; memsize=memsize*2 )
  // {
  //   int testing_size = memsize/4;
  //   measure_access_time(testing_size, stridesize);
  // }


  /*
  * test v.2
  */
  stridesize = (int) pow(2,13); //8KB
  // cout << "<Stride = >" << stridesize << endl;
  for(long long int memsize = minsize; memsize <=maxsize; memsize=memsize*1.3 )
  {
    int testing_size = memsize/4;
    measure_access_time(testing_size, stridesize);
  }
  
  /*
  * debug
  */
  // measure_access_time(9, 3);
}