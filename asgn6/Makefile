SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm

all: encode decode

encode: encode.o trie.o word.o io.o
	$(CC) $(CFLAGS) $(LDFLAGS) encode.o trie.o word.o io.o -o encode

decode: decode.o trie.o word.o io.o
	$(CC) $(CFLAGS) $(LDFLAGS) decode.o trie.o word.o io.o -o decode
	
encode.o: encode.c trie.h word.h io.h
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c trie.h word.h io.h
	$(CC) $(CFLAGS) -c decode.c

trie.o: trie.c trie.h
	$(CC) $(CFLAGS) -c trie.c

word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[c,h]
