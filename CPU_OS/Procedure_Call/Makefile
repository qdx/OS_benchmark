proc: proc.o
	g++ -o proc proc.o -lrt
proc.o: proc.cpp cycle.h
	g++ -c proc.cpp
clean: 
	rm *.o
	rm proc
