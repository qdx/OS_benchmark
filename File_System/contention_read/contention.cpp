#include <pthread.h>
#include "cycle.h"
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define ITERATIONS 10
#define BLOCK_SIZE 4096

const int MAX_THREAD = 10;
const char *FNAME_PREFIX= "f";
const int FILE_SIZE = 1024*1024*8; //1MB file
char* filename_array[MAX_THREAD];

int file_name_counter = 0;
double critical_time_sum = 0.0;
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;


int file_exist(char *filename){
  struct stat buffer;
  return (stat(filename, &buffer) == 0);
}

void makeFile(long long int size, char* filename)
{
  if(file_exist(filename)) return;
  char* arr = new char[size];

  // char arr[size];

  int x;
  for(x = 0; x<size; x++)
  {
    arr[x] = 'a';
  }

  ofstream myOutFile;
  myOutFile.open(filename);

  if(myOutFile.is_open())
  {
    for(x=0; x<size; x++)
    {
      myOutFile << arr[x] << endl;
    }
    myOutFile.close();
  }

  delete[] arr;
}

int file_sequential_read(long long int size, char* filename) {

  ticks t1, t2;
  long long int totalTime;
  int fd;
  char* buffer; //= new char[size];
  int page = getpagesize();

  posix_memalign((void**) &buffer, page, size);
  fd = open(filename, O_RDONLY | O_DIRECT);
  if(fd != -1)
  {
    t1 = getticks();
    lseek(fd, 0, SEEK_CUR);
    read(fd, buffer, size);
    t2 = getticks();

    if(errno != 0)
    {
      cout << "errno: " << errno << endl;
      return -1;
    }

    //depending on the size of the file, you may need to change this
    //so that it doesn't give stack overflow
    totalTime = ((elapsed(t2,t1) - 45)*0.417)/ITERATIONS; 
  }
  else
  {
    cout << "file not opened" << endl;
  }

  close(fd);
  delete[] buffer;

  return totalTime;
}



void *run_readfile(void *ptr)
{
  //sequential access
  long long int stotal;

  //cout << "size (KB)  " << "Time/block (ns) S " << "Time/byte (ns) R "<< endl;
  //change the numbers here to get more size stuff

  int i;
  stotal = 0;
  long long int converted_size = FILE_SIZE;
  for(i = 0; i < ITERATIONS; i++)
  {
    long long int sAnswer = file_sequential_read(converted_size, (char*)ptr);
    stotal += sAnswer;
  }
  long long int bytePerBlock = ((long) BLOCK_SIZE)/((long) sizeof(char));
  long long int sFinal = (stotal)/converted_size*bytePerBlock;

  pthread_mutex_lock(&sum_mutex);
  critical_time_sum += sFinal;
  pthread_mutex_unlock(&sum_mutex);
}

double contention_file_read(int number_of_threads){
  pthread_t thread_id[MAX_THREAD];
  critical_time_sum = 0.0;
  int i,j;

  for(i = 0 ; i < number_of_threads; i ++){
    pthread_create( &thread_id[i] , NULL, run_readfile, filename_array[i]);
  }
  for(j = 0 ; j < number_of_threads; j ++){
    pthread_join(thread_id[j], NULL);
  }
  return critical_time_sum / number_of_threads;
}

int main(int argc, const char* argv[]){
  int k;
  for(k = 0 ; k < MAX_THREAD; k ++){
    char buffer[10];
    sprintf(buffer, "%d", file_name_counter);
    filename_array[k] = new char[50];
    strcpy(filename_array[k], FNAME_PREFIX);
    strcat(filename_array[k], buffer);
    file_name_counter ++;
    makeFile(FILE_SIZE, filename_array[k]);
  }

  cout << contention_file_read(2) << endl;
  cout << contention_file_read(3) << endl;
  cout << contention_file_read(4) << endl;
  cout << contention_file_read(5) << endl;
  cout << contention_file_read(6) << endl;
  cout << contention_file_read(7) << endl;
  cout << contention_file_read(8) << endl;
  cout << contention_file_read(9) << endl;
  cout << contention_file_read(10) << endl;

  for(k = 0 ; k < MAX_THREAD; k ++){
    delete[] filename_array[k];
  }
  exit(0);
}
