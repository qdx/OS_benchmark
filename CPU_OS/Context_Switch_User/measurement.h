#include<stdlib.h>
#include<math.h>
static inline unsigned long long int RDTSC() {
        unsigned int hi, lo;
        asm volatile("rdtsc;" : "=a" (lo), "=d" (hi));
        return ((unsigned long long int) hi << 32) | lo;
}

#define MEASUREMENT(LOOPCOUNT, FUNC, NAME, LOWERLIMIT, UPPERLIMIT)\
{\
        unsigned long long start, end, sumtime = 0;\
        int i, *timetable, counter = 0;\
        double average, sdev = 0;\
        timetable = (int *)malloc(LOOPCOUNT * sizeof(int));\
        for(i = 0; i < LOOPCOUNT; i++){\
                start = RDTSC();\
                FUNC;\
                end = RDTSC();\
                if(end - start < UPPERLIMIT && end - start > LOWERLIMIT){\
                        timetable[counter] = end - start;\
                        sumtime += end - start;\
                        counter++;\
                }\
        }\
        average = (double) sumtime / counter;\
        for(i = 0; i < counter; i++){\
                sdev += (pow(((double)timetable[i] - average), 2));\
        }\
        sdev = pow(sdev / counter, .5);\
        printf("%s: executed %d times, %.2lf%% of them are in range (%d, %d), average %.2lf cycles, standard devition %.2lf cycles.\n",\
                        NAME, LOOPCOUNT, (double) counter / LOOPCOUNT * 100, LOWERLIMIT, UPPERLIMIT, average, sdev);\
}\

#define MEASUREMENT10X(LOOPCOUNT, FUNC, NAME, LOWERLIMIT, UPPERLIMIT)\
{\
        unsigned long long start, end, sumtime = 0;\
        int i, *timetable, counter = 0;\
        double average, sdev = 0;\
        timetable = (int *)malloc(LOOPCOUNT * sizeof(int));\
        for(i = 0; i < LOOPCOUNT; i++){\
                start = RDTSC();\
                FUNC;\
                FUNC;\
                FUNC;\
                FUNC;\
                FUNC;\
                FUNC;\
                FUNC;\
                FUNC;\
                FUNC;\
                FUNC;\
                end = RDTSC();\
                if(end - start < UPPERLIMIT && end - start > LOWERLIMIT){\
                        timetable[counter] = end - start;\
                        sumtime += end - start;\
                        counter++;\
                }\
        }\
        average = (double) sumtime / counter;\
        for(i = 0; i < counter; i++){\
                sdev += (pow(((double)timetable[i] - average), 2));\
        }\
        sdev = pow(sdev / counter, .5);\
        printf("%s: executed %d * 10 times, %.2lf%% of them are in range (%d, %d), average %.2lf cycles, standard devition %.2lf cycles.\n",\
                        NAME, LOOPCOUNT, (double) counter / LOOPCOUNT * 100, LOWERLIMIT, UPPERLIMIT, average, sdev);\
}\