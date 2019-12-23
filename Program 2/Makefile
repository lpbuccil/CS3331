CC=gcc
OPTS= -g -std=c89 -pedantic -Wall -I. 

default: main merge

main: main.c
	$(CC) $(OPTS) main.c -o main -lm
merge: merge.c
	$(CC) $(OPTS) merge.c -o merge -lm

.PHONY: clean

clean:
	rm -f main
	rm -f merge