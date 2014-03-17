CFLAGS= -g -lm
CC=gcc
SRC=mem_read_bw.c
AS=nasm

membw:	mem_read_bw.c read_mem.asm
	${AS} -f elf read_mem.asm -o read_mem.o
	${CC} -funroll-loops -m32 -mtune=pentium read_mem.o ${SRC} ${CFLAGS} -o bwmem

clean: 
		rm bwmem
		rm read_mem.o

