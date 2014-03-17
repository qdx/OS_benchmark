//on dexin ubuntu

#include <stdio.h>
#include <iostream>
#include "cycle.h"
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstdlib> 
#include <ctime> 

#define ITERATIONS 10
#define BLOCK_SIZE 4096

using namespace std;


long long int random(int max)
{
    srand((unsigned)time(0)); 
    long long int random_integer = (rand()%max)+1; 
    return random_integer; 
}

long long * rpermute(int n) {
    long long *a = new long long[n*8];
    long long k;
    for (k = 0; k < n; k++)
	a[k] = k;
    for (k = n-1; k > 0; k--) {
		long long j = rand() % (k+1);
		long long temp = a[j];
		a[j] = a[k];
		a[k] = temp;
    }
    return a;
}

int overhead()
{
	ticks t1, t2;
	t1 = getticks();
	int i;
	for(i=0; i<ITERATIONS; i++)
	{
		i+=0;
	}
	t2 = getticks();
	return elapsed(t2,t1) - 45;
}

int file_random_read(long long int size) {

	ticks t1, t2;
	long long int totalTime;
	long long int finalTime;
	int fd;
	long long* buffer; //= new char[size];
	int page = getpagesize();

	posix_memalign((void**) &buffer, page, size);
	fd = open("testFile.txt", O_RDONLY | O_DIRECT | O_SYNC);
	if(fd != -1)
	{
		long long n = size / BLOCK_SIZE;
		long long *seq = rpermute(n);

		
		int i;
		for(i=0; i<10; i++)
		{
			t1 = getticks();
			lseek(fd, seq[i]*BLOCK_SIZE, SEEK_SET);
			read(fd, buffer, BLOCK_SIZE);
			t2 = getticks();
			totalTime += (elapsed(t2,t1) - 45);
		}
		
		if(errno != 0)
		{
			cout << "errno: " << errno << endl;
			return -1;
		}



		//depending on the size of the file, you may need to change this
		//so that it doesn't give stack overflow
		finalTime = (((totalTime)/10)*0.47)/ITERATIONS;
	}


	
	else
	{
		cout << "file not opened" << endl;
	}

	close(fd);
	delete[] buffer;
	
	return finalTime;

}

void makeFile(long long int size, string filename)
{
	char* arr = new char[size];

	// char arr[size];

	int x;
	for(x = 0; x<size; x++)
	{
		arr[x] = 'a';
	}

	ofstream myOutFile;
	myOutFile.open(filename.c_str());

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


int main(int argc, const char* argv[])
{
	//sequential access
	int conversion = 1024;
	long long int fileSize;
	string testFileName = "testFile.txt";
	long long int stotal, rtotal;


	cout << "size (KB)  " << "Random Time/block (ns)"<< endl;
	//change the numbers here to get more size stuff
	for(fileSize = 50; fileSize < 3000000; fileSize=fileSize*2) //this is all in KB
	{
		int i;
		stotal = 0;
		rtotal = 0;
		long long int converted_size = fileSize*conversion;
		makeFile(converted_size, testFileName);
		for(i = 0; i < ITERATIONS; i++)
		{
			// long long int sAnswer = file_sequential_read(converted_size);
			long long int rAnswer = file_random_read(converted_size);
			// stotal += sAnswer;
			rtotal += rAnswer;
		}
		long long int bytePerBlock = ((long) BLOCK_SIZE)/((long) sizeof(char));
		long long int sFinal = (stotal)/converted_size*bytePerBlock;
		long long int rFinal = (rtotal);//converted_size*bytePerBlock;

		cout << fileSize << "          " << rFinal << endl; //this time is printed in ns
	}
	
}

//on dexin ubuntu
