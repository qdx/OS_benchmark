#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include "cycle.h"

#define LOOP_SIZE 1000
#define PIPE_LOOP_INNER 1e4
#define PIPE_LOOP_OUTER 100

uint64 findPipeOverhead(int* pipefd)
{
tick_init();
uint64 tick1, tick2, diff;

int i, j;
uint64 sum = 0;
double sqSum = 0;

for (i = 0; i < PIPE_LOOP_OUTER; i++)
{
tick(tick1);
for (j = 0; j < PIPE_LOOP_INNER; j++)
{
write(pipefd[1], &sum, sizeof(uint64));
read(pipefd[0], &sum, sizeof(uint64));  
}
tick(tick2);
diff = tick_diff_loop(tick1, tick2, PIPE_LOOP_INNER);
//printf("pipe diff: %llu, %f\n", diff, tick_time(tick1, tick2));
sum += diff;
sqSum += diff * diff;
}

sum /= PIPE_LOOP_INNER;
sqSum = sqSum / PIPE_LOOP_INNER / PIPE_LOOP_INNER;

uint64 avg = sum / PIPE_LOOP_OUTER;
printf("Pipe overhead: ");
print_stat((double)sum, sqSum, PIPE_LOOP_OUTER);

return avg;
}

int main_proc ( int argc, char *argv[] )
{
int pipefd[2];
pipe(pipefd);

uint64 pipeOverhead = findPipeOverhead(pipefd);

uint64 childTick, tick1, tick2, diff;

tick_init();

//FILE* fp = fopen("context_switch.csv", "w");

int i;
double sum = 0, sqSum = 0;
for (i = 0; i < LOOP_SIZE; i++)
{
pid_t pid = fork();
if (pid == 0) //child
{
//printf("C Hello from child.\n");
tick(tick1);  //we include the write time because it is easier to measure overhead including both writing and reading
write(pipefd[1], &tick1, sizeof(uint64));
sched_yield();
exit(0);
}
else  //parent
{
//printf("P Hello from parent, created child %d\n", pid);
read(pipefd[0], &childTick, sizeof(uint64));
tick(tick2);
diff = tick_diff(childTick, tick2) - pipeOverhead;

//for data analysis
//fprintf(fp, "%d,%llu\n", i, diff);
//fflush(fp);

sum += diff;
sqSum += diff * diff;

waitpid(pid, NULL, 0);
//To test that children are not interweaved.
/*pid_t wpid = wait(NULL);
if (wpid != pid)
{
printf("pid=%d, wpid=%d\n", pid, wpid);
exit(-1);
}*/
}
}

printf("Context switch time: ");
print_stat(sum, sqSum, LOOP_SIZE);

//fclose(fp);

return 0;
} 

void* thread_worker(void* ticket)
{
unsigned tick_eax, tick_edx;  //we cannot use tick_init() here because it will estimate overhead
uint64 tick1;
tick(tick1);
write(((int*)ticket)[1], &tick1, sizeof(uint64));
//printf("T write: %llu\n", tick1);
pthread_yield();
}

int main (int argc, char* argv[])
{
tick_init();
pthread_t thread;

FILE* fp = fopen("context_switch_thread.csv", "w");

int pipefd[2];
pipe(pipefd);

uint64 pipeOverhead = findPipeOverhead(pipefd);

int i;
uint64 tick1, tick2, diff;
double sum, sqSum;

for (i = 0; i < LOOP_SIZE; i++)
{
pthread_create(&thread, NULL, thread_worker, (void*)pipefd);
read(pipefd[0], &tick1, sizeof(uint64));
//printf("P read: %llu\n", tick1);
tick(tick2);
diff = tick_diff(tick1, tick2) - pipeOverhead;  
fprintf(fp,"%d,%llu\n", i, diff);
fflush(fp);

sum += diff;
sqSum += diff * diff;
}

print_stat(sum, sqSum, LOOP_SIZE);

fclose(fp);
return 0;
}
