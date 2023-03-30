SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: life
life: life.o universe.o universe.h
	$(CC) -lm -lncurses -o life life.o universe.o

life.o: life.c
	$(CC) $(CFLAGS) -c life.c

universe.o: universe.c
	$(CC) $(CFLAGS) -c universe.c

clean:
	rm -f life *.o 

format:
	clang-format -i -style=file *.[ch]
