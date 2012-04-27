CC:=gcc
OPTS:=-pedantic -Wall -Wextra -O3

.PHONY: clean

all: alloc_test

alloc_test: alloc_test.c
	$(CC) $(OPTS) -o $@ $^

clean:
	-rm -rf alloc_test

