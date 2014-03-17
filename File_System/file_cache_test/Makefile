file_cache_test: file_cache_test.o
	g++ -o file_cache_test file_cache_test.o
file_cache_test.o: file_cache_test.cpp cycle.h
	g++ -c file_cache_test.cpp
clean: 
	rm *.o
