mmap: mmap.o
	gcc -o mmap mmap.o
mmap.o: mmap.c cycle.h
	gcc -c mmap.c
clean: 
	rm *.o
	rm mmap
