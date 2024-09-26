.PHONY: clean
CC=gcc
CFLAGS=-std=c17 -Wpedantic -Wall

experiment: experiment.c screen.c
	${CC} experiment.c screen.c -o experiment -l SDL2 ${CFLAGS}

clean:
	rm experiment