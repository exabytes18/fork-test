#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int MB;
	int64_t *data;
	char *argp;
	size_t i, bytes;
	pid_t child_pid;
	struct timeval start, end;
	time_t elapsed_time_us;
	double elapsed_time;
	
	if(argc != 2) {
		printf("usage: ./fork #MB\n");
		exit(EXIT_FAILURE);
	}
	
	errno = 0;
	MB = strtol(argv[1], &argp, 0);
	if(errno != 0) {
		perror("Illegal MB value");
		exit(EXIT_FAILURE);
	} else if(argv[1] == argp || MB < 1) {
		printf("Invalid MB value: must be positive integer\n");
		exit(EXIT_FAILURE);
	}
	
	bytes = (size_t)MB * (1024 * 1024);
	
	/* Slash and burn; don't worry about free'ing */
	data = malloc(bytes);
	if(data == NULL) {
		printf("Failed to malloc %d MB of memory\n", MB);
		exit(EXIT_FAILURE);
	}
	
	/* Init memory to keep kernel honest */
	for(i = 0; i < bytes / sizeof(data[0]); i++) {
		data[i] = (int64_t)i;
	}
	
	/* Fork */
	gettimeofday(&start, NULL);
	child_pid = fork();
	gettimeofday(&end, NULL);
	
	/* Make sure our memory is ok */
	for(i = 0; i < bytes / sizeof(data[0]); i++) {
		if(data[i] != (int64_t)i) {
			printf("Uh oh, weird memory...\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/* Calculate time to execute fork, as witnessed by parent and child */
	elapsed_time_us = end.tv_usec - start.tv_usec;
	elapsed_time_us += 1000000l * (end.tv_sec - start.tv_sec);
	elapsed_time = (double)(elapsed_time_us / 1000000.);
	
	if(child_pid == 0) {
		printf("fork() to child: %.3f\n", elapsed_time);
		exit(EXIT_SUCCESS);
	} else {
		waitpid(child_pid, NULL, 0);
		printf("fork() to parent: %.3f\n", elapsed_time);
	}
	
	return EXIT_SUCCESS;
}

