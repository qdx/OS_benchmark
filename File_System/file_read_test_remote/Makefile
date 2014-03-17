all: file_seq_read_test file_rand_read_test

file_rand_read_test: file_rand_read_test.o
	g++ -o file_rand_read_test file_rand_read_test.o
file_rand_read_test.o: file_rand_read_test.cpp cycle.h
	g++ -c file_rand_read_test.cpp

file_seq_read_test: file_seq_read_test.o
	g++ -o file_seq_read_test file_seq_read_test.o
file_seq_read_test.o: file_seq_read_test.cpp cycle.h
	g++ -c file_seq_read_test.cpp
clean: 
	rm *.o
