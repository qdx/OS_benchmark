all: fork.o
	g++ -pthread thread.cpp -o thread
	g++ -o fork fork.o

fork.o: fork.cpp cycle.h
	g++ -c fork.cpp -ltr

fork: fork.o
	g++ -o fork fork.o

thread: 
	g++ -pthread thread.cpp -o thread

clean:
	rm fork
	rm thread
	rm *.o
