main: main.o
	g++ -o main main.o -lrt
main.o: main.cpp measurement.h
	g++ -c main.cpp
clean: 
	rm *.o
	rm main

