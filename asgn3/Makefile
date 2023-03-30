SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: sorting
sorting: sorting.o batcher.o shell.o heap.o quick.o set.o stats.o
	$(CC) -lm -o sorting sorting.o batcher.o shell.o heap.o quick.o set.o stats.o

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c

batcher.o: batcher.c
	$(CC) $(CFLAGS) -c batcher.c

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c

clean:
	rm -f sorting *.o 

format:
	clang-format -i -style=file *.[ch]
