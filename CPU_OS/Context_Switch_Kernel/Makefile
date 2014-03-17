contextSwitch: contextSwitch.o
	gcc -o contextSwitch contextSwitch.o -lm -lpthread
contextSwitch.o: contextSwitch.c cycle.h
	gcc -c contextSwitch.c
clean: 
	rm *.o
	rm contextSwitch
