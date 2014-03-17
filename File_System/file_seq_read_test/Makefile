file_read_test: file_read_test.o
	g++ -o file_read_test file_read_test.o
file_read_test.o: file_read_test.cpp cycle.h
	g++ -c file_read_test.cpp
clean: 
	rm *.o
