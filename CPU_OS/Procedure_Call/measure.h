#include "cycle.h"

#define MEASURE(iteration_times, FUNC){\
  double sum_ticks = 0.0;\
  for(int i = 0 ; i < iteration_times ; i++){\
    ticks t1 = getticks();\
    FUNC;\
    ticks t2 = getticks();\
    sum_ticks += elapsed(t2,t1);\
  }\
  return sum_ticks/(double)iteration_times;\
}\

