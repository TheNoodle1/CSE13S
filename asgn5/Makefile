# Compiler and flags
SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm
LIBS = -lgmp -lncurses

# Targets
all: encrypt decrypt keygen

encrypt: encrypt.o ss.o numtheory.o randstate.o
	$(CC) $(CFLAGS) $(LDFLAGS) encrypt.o ss.o numtheory.o randstate.o $(LIBS) -o encrypt

decrypt: decrypt.o ss.o numtheory.o randstate.o
	$(CC) $(CFLAGS) $(LDFLAGS) decrypt.o ss.o numtheory.o randstate.o $(LIBS) -o decrypt

keygen: keygen.o ss.o numtheory.o randstate.o
	$(CC) $(CFLAGS) $(LDFLAGS) keygen.o ss.o numtheory.o randstate.o $(LIBS) -o keygen

numtheory.o: numtheory.c numtheory.h
	$(CC) $(CFLAGS) -c numtheory.c

randstate.o: randstate.c randstate.h numtheory.h
	$(CC) $(CFLAGS) -c randstate.c

ss.o: ss.c ss.h randstate.h numtheory.h
	$(CC) $(CFLAGS) -c ss.c

encrypt.o: encrypt.c ss.h numtheory.h randstate.h
	$(CC) $(CFLAGS) -c encrypt.c

decrypt.o: decrypt.c ss.h numtheory.h randstate.h
	$(CC) $(CFLAGS) -c decrypt.c

keygen.o: keygen.c ss.h numtheory.h randstate.h
	$(CC) $(CFLAGS) -c keygen.c

clean:
	rm -f encrypt decrypt keygen *.o

format:
	clang-format -i -style=file *.[ch]
