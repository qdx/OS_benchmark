//On Dexin Ubuntu

#include <stdio.h>
#include <iostream>
#include "cycle.h"
#include <fstream>
#include <string.h>


#define ITERATIONS 10
using namespace std;

int test_file_cache(string &filename) {

	long long int total;
	long long int overhead;
	int iterations;
	long long int temp;
	ifstream myReadFile;
	int i = 0;
	ticks t1, t2, t1overhead, t2overhead;

	myReadFile.open(filename.c_str(),ios::in);

	if (myReadFile.is_open()) {
		string line;
		t1 = getticks();
		for(i = 0; i<ITERATIONS; i++)
		{
			getline(myReadFile, line);
		}
		t2 = getticks();
		myReadFile.close();
	}
	total = elapsed(t2,t1);

	// calculate overhead time
	t1overhead = getticks();
	for(i = 0; i<ITERATIONS; i+=0)
	{
		i++;
	}
	t2overhead = getticks();
	overhead = elapsed(t2overhead,t1overhead);
	//end overhead calculation

	return ((total-overhead)*0.47);
}

int test_file_cache2 (string &filename) {

	long long int total;
	long long int overhead;
	int iterations;
	long long int temp;
	ifstream myReadFile;
	int i = 0;
	ticks t1, t2, t1overhead, t2overhead;

	myReadFile.open(filename.c_str(),ios::in);

	if (myReadFile.is_open()) {
		string line;
		t1 = getticks();
		for(i = 0; i<ITERATIONS; i++)
		{
			getline(myReadFile, line);
		}
		t2 = getticks();
		myReadFile.close();
	}
	total = elapsed(t2,t1);

	// calculate overhead time
	t1overhead = getticks();
	for(i = 0; i<ITERATIONS; i+=0)
	{
		i++;
	}
	t2overhead = getticks();
	overhead = elapsed(t2overhead,t1overhead);
	//end overhead calculation

	return ((total-overhead)*0.47)/ITERATIONS;
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
	int conversion = 1024;
	string filename = "5Mtest.txt";
	string testFileName = "testFile.txt";
	
	int fileSize;
	//1.5GB file cache = 1500MB cache = 1536000 KB
	//start from 50KB
	cout << "size (KB)  " << "Time (ms)"<< endl;
	for(fileSize = 50; fileSize < 3000000; fileSize=fileSize*2) //this is all in KB
	{
		long long int converted_size = fileSize*conversion;
		makeFile(converted_size, testFileName);
		long long int answer = test_file_cache(testFileName);
		// cout << "read time for " << fileSize <<"KB file : " << endl;
		cout << fileSize << "          " << answer << endl; //this time is printed in ms
	}
	
}

//On Dexin Ubuntu