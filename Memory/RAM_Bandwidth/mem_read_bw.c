#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cycle.h"

const int READ = 1;
const int WRITE = 2;

// This is deprecated
int read_mem_with_stride(int size, int stride){
  unsigned char * chunk;
  unsigned char *chunk0;
  unsigned long tmp;
  chunk0 = chunk = malloc (size+128);

  tmp = (unsigned long) chunk;
  if (tmp & 0b111111) {
    tmp -= (tmp & 0b111111);
    tmp += 0b1000000;
    chunk = (unsigned char*) tmp;
  }

  memset (chunk, 0, size);

  int i = 0;
  double etime = 0.0;
  int c = 0;
  ticks t1 = getticks();
  mem_read_word_stride(chunk, size, stride);
  ticks t2 = getticks();
  etime = elapsed(t2, t1);
  printf("%f\n", etime/((double)size/(stride)));
  /*printf("%d: %f\n",c, etime);*/
  /*c++;*/

  free ((void*)chunk0);
  return 1;

}

double read_mem(unsigned long size, int loops, int mode){

  // chunk points to start of the aligned memory
  // chunk0 points to start of the original malloc memory
  unsigned char *chunk;
  unsigned char *chunk0;
  unsigned long tmp;

  // we are reading 128 bit per read, so the size 
  // have to be multiples of 128
  if (size & 127)
    error ("do_read(): chunk size is not multiple of 128.");

  // adding 64 so that when aligning the memory to 
  // boundary of 32, the access won't exceed boundary
  chunk0 = chunk = malloc (size+64);
  if (!chunk) 
    error ("Out of memory");

  // init memory
  memset (chunk, 0, size);

  // align memory address to 32bit boundary,
  // to satisfy the hint requirement of movntdqa
  tmp = (unsigned long) chunk;
  if (tmp & 0b11111) {
    tmp -= (tmp & 0b11111);
    tmp += 0b100000;
    chunk = (unsigned char*) tmp;
  }

  double etime = 0.0;
  if(mode == READ){
    ticks t1 = getticks();
    memory_read_128bit(chunk, size, loops);
    ticks t2 = getticks();
    etime = elapsed(t2, t1);
  }else{
    ticks t1 = getticks();
    memory_write_128bit(chunk, size, loops);
    ticks t2 = getticks();
    etime = elapsed(t2, t1);

  }

  free ((void*)chunk0);
  return etime;
}

// This is deprecated
void fetch_analysis(){
  int stride = 1;
  int vsize = 256;
  int i = 0;
  for(i = 1 ; i <= 1024*1024; i *=2, vsize*=2){
    /*printf("stride:%d\n",i);*/
    read_mem_with_stride(vsize, i);
  }
}

void bandwidth_test(int max_size, int iteration, int loops, int mode){

  int size = 128;// bytes

  while(size <= max_size){
    int i = 0;
    double sum = 0.0;
    for(i = 0 ; i < iteration; i ++){
      sum += read_mem(size,loops,mode);
    }
    double mean = sum/iteration;
    double size_in_mb = (double)size/(1024*1024);
    double bw = (size_in_mb*loops*1e9)/((mean)*0.417);

    printf("%f\n", bw);
    /*printf("size:%d, time:%f, bandwidth:%f\n", size, mean, bw);*/
    size = size*2;
  }

}

// This is deprecated
double read_write(unsigned long size, int loops){

  // chunk points to start of the aligned memory
  // chunk0 points to start of the original malloc memory
  unsigned char *chunk;
  unsigned char *chunk0;
  unsigned long tmp;

  // we are reading 128 bit per read, so the size 
  // have to be multiples of 128
  if (size & 127)
    error ("do_read(): chunk size is not multiple of 128.");

  // adding 64 so that when aligning the memory to 
  // boundary of 32, the access won't exceed boundary
  chunk0 = chunk = malloc (size+64);
  if (!chunk) 
    error ("Out of memory");

  // init memory
  memset (chunk, 0, size);

  // align memory address to 32bit boundary,
  // to satisfy the hint requirement of movntdqa
  tmp = (unsigned long) chunk;
  if (tmp & 0b11111) {
    tmp -= (tmp & 0b11111);
    tmp += 0b100000;
    chunk = (unsigned char*) tmp;
  }

  double etime = 0.0;
  ticks t1 = getticks();
  memory_read_128bit(chunk, size, loops);
  ticks t2 = getticks();
  etime = elapsed(t2, t1);

  free ((void*)chunk0);
  return etime;
}

// This is deprecated
void latency_L1(int size){

  unsigned char* data;
  data = malloc(size);

  int i = 0;

  for(i = 0 ; i < 100 ; i ++){
    ticks t1 = getticks();
    memory_read_byte(data);
    ticks t2 = getticks();
    double etime = 0.0;
    etime = elapsed(t2, t1)-44;
    printf("%f\n", etime);
  }

  free((void*)data);
}

int main(int argc, char* argv[]){
  // max read size is 64MB
  int max_size = 1048576 * 64;
  int iteration = 100;
  int loops = 100;
  bandwidth_test(max_size, iteration, loops, READ);
  bandwidth_test(max_size, iteration, loops, WRITE);

  /*int i = 0;*/
  /*for(i=0; i<loops; i++){*/
  /*printf("========\n");*/
  /*latency_L1(512);*/
  /*}*/

  return 1;
}

