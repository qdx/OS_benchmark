#define LOOP_COUNT 10000
#include <stdio.h>
#include "cycle.h"

int main(int argc, char *argv[])
{
// float avg = 0.0;
// int sum = 0;
int i;

//for (i = 0; i < LOOP_COUNT; ++i)
        for (;;)
        {
                ticks t1 = getticks();
                ticks t2 = getticks();
                printf("%f, %llu\n", elapsed(t2,t1), t2-t1);
               // sum += (int)(t2-t1);
        }
// avg = sum / LOOP_COUNT;
// printf("avg = %f\n", avg);
        return 0;
}
