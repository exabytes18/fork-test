CC:=gcc
OPTS:=-pedantic -Wall -Wextra -O3

.PHONY: clean

all: fork

fork: fork.c
	$(CC) $(OPTS) -o $@ $^

clean:
	-rm -rf fork

