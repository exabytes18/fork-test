#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int64_t *alloc_and_init(size_t bytes) {
	size_t i;
	int64_t *data;

	data = malloc(bytes);
	if(data == NULL) {
		printf("Failed to malloc %ld bytes of memory\n", (unsigned long int) bytes);
		exit(EXIT_FAILURE);
	}

	/* Init memory to keep kernel honest */
	for(i = 0; i < bytes / sizeof(data[0]); i++) {
		data[i] = (int64_t)i;
	}

	return data;
}

void check_mem(int64_t *data, size_t bytes) {
	size_t i;

	for(i = 0; i < bytes / sizeof(data[0]); i++) {
		if(data[i] != (int64_t)i) {
			printf("Uh oh, weird memory...\n");
			exit(EXIT_FAILURE);
		}
	}
}

/*
 * Allocates a chunk of memory of size "bytes" and initializes it to something
 * arbitrary. Then, fork the process.
 *
 * The reported times are the amount of time it took for the fork call to
 * return to the child and the parent.
 */
void fork_test(size_t bytes) {
	int64_t *data;
	pid_t child_pid;
	struct timeval start, end;
	time_t elapsed_time_us;
	double elapsed_time;

	data = alloc_and_init(bytes);

	gettimeofday(&start, NULL);
	child_pid = fork();
	gettimeofday(&end, NULL);

	if(child_pid == -1) {
		perror("Fork");
		free(data);
		exit(EXIT_FAILURE);
	} else {
		check_mem(data, bytes);
		free(data);
	}

	/* Calculate time to execute fork, as witnessed by parent and child */
	elapsed_time_us = end.tv_usec - start.tv_usec;
	elapsed_time_us += 1000000l * (end.tv_sec - start.tv_sec);
	elapsed_time = (double)(elapsed_time_us / 1000000.);

	if(child_pid == 0) {
		printf("Time for fork() to return to child:  %.3f\n", elapsed_time);
		exit(EXIT_SUCCESS);
	} else {
		waitpid(child_pid, NULL, 0);
		printf("Time for fork() to return to parent: %.3f\n", elapsed_time);
	}
}

/*
 * Allocates a chunk of memory of size "bytes" and initializes it to something
 * arbitrary. Then, allocate a second chunk of memory of size "bytes" but don't
 * initialize it.
 *
 * The reported time is the amount of time it took to alloc the second chunk of
 * memory.
 */
void uninitialized_alloc_test(size_t bytes) {
	int64_t *data, *uninitialized_memory;
	struct timeval start, end;
	time_t elapsed_time_us;
	double elapsed_time;

	data = alloc_and_init(bytes);

	gettimeofday(&start, NULL);
	uninitialized_memory = malloc(bytes);
	gettimeofday(&end, NULL);
	
	free(data);
	
	if(uninitialized_memory == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	} else {
		free(uninitialized_memory);
	}

	elapsed_time_us = end.tv_usec - start.tv_usec;
	elapsed_time_us += 1000000l * (end.tv_sec - start.tv_sec);
	elapsed_time = (double)(elapsed_time_us / 1000000.);
	
	printf("Time to malloc after initialization: %.3f\n", elapsed_time);
}

int main(int argc, char **argv) {
	int MB;
	char *argp;
	size_t bytes;

	if(argc != 3) {
		printf("usage: ./alloc_test [F|A] #MB\n");
		exit(EXIT_FAILURE);
	}

	errno = 0;
	MB = strtol(argv[2], &argp, 0);
	if(errno != 0) {
		perror("Illegal MB value");
		exit(EXIT_FAILURE);
	} else if(argv[2] == argp || MB < 1) {
		printf("Invalid MB value: must be positive integer\n");
		exit(EXIT_FAILURE);
	}

	bytes = (size_t)MB * (1024 * 1024);

	if(strcmp(argv[1], "F") == 0) {
		fork_test(bytes);
	} else if(strcmp(argv[1], "A") == 0) {
		uninitialized_alloc_test(bytes);
	} else {
		printf("Unknown option: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

