#include <sys/mman.h>
// mmap
#include <sys/types.h>
// open, fstat
#include <sys/stat.h>
// open, fstat
#include <fcntl.h>
// open
#include <stdio.h>
// perror
#include <stdlib.h>
// exit
#include <unistd.h>
// close, fstat, sysconf
#include <string.h>
// strncpy
#include "cycle.h"

int main(int argc, char *argv[]) {

	long pagesize = sysconf(_SC_PAGE_SIZE);
	// fprintf(stdout, "page size: %ld\n", pagesize);

	int i;
	for (i = 0; i < 10000; ++i) {
		int fd;
		struct stat buf;
		void* mmapp, *mmapp_bkp;

		fd = open("mmap_file", O_RDONLY);
		if (fd == -1) {
			perror("open");
			exit(1);
		}

		if (fstat(fd, &buf) < 0) {
			perror("fstat");
			close(fd);
			exit(1);
		}

		mmapp = mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
		mmapp_bkp = mmapp;

		if (mmapp == MAP_FAILED) {
			perror("mmap");
			if (buf.st_size == 0) {
				fprintf(stderr, "file size = 0\n");
			}
			close(fd);
			exit(1);
		}

		int j;
		char c;
		ticks t1, t2;
		for (j = 0; j < 100; ++j) {
			t1 = getticks();
			c = *(char*)mmapp;
			t2 = getticks();
			mmapp += 4096;
			fprintf(stdout, "%llu\n", t2 - t1);
		}

		//munmap(mmapp, buf.st_size);
		if (munmap(mmapp_bkp, buf.st_size) == -1) {
			perror("munmap");
		}
		// strcpy(mmapp, "strncpy test");
		close(fd);

	}
	return 0;
}
