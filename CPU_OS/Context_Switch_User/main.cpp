#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "measurement.h"

#define LOOPCOUNT 10000

void contextSwitchOverhead()
{
        unsigned long long start_size;
        unsigned long long start1, end1, child1;
        unsigned long long start12, end12, child12;
        double actual_sum = 0;
        double overhead_sum = 0;

        int fd[2];
        pipe(fd);
        // char data = 'a';
        // char buf[100];


        //actual measurement part
        start_size = RDTSC();
        for(int i = 0; i < LOOPCOUNT; i++)
        {
                int pid = fork();
                if(pid == 0){
                        // printf("C Hello from child.\n");
                        start1 = RDTSC();
                        write(fd[1], &start1, sizeof(start_size));
                        _exit(0);
                }
                else
                {
                        // printf("P Hello from parent, created child %d\n", pid);
                        read(fd[0], &child1, sizeof(start_size));
                        end1 = RDTSC();
                        waitpid(pid, NULL, 0);   
                        // printf("P Finished running!\n");
                }
                actual_sum += end1 - child1;
        
        }
        

        //overhead calculation
        for(int i = 0; i < LOOPCOUNT; i++)
        {
                start12 = RDTSC();
                write(fd[1], &start12, sizeof(start_size));
                read(fd[0], &child12, sizeof(start_size));
                end12 = RDTSC();
                overhead_sum += end12 - child12;
        }
        // printf("overhead time: %f\n",overhead_sum);

        double sum = actual_sum - overhead_sum;
        // printf("sum: %f\n", sum);
        // printf("process context switch: %f\n", sum / LOOPCOUNT);

        printf("%f\n", sum / LOOPCOUNT);
        
}

int main(){
        // forkOverhead();
        contextSwitchOverhead();
        return 0;
}