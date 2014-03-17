ram_access_time: ram_access_time.o
	g++ -o ram_access_time ram_access_time.o
ram_access_time.o: ram_access_time.cpp cycle.h
	g++ -c ram_access_time.cpp
clean: 
	rm ram_access_time
	rm ram_access_time.o
