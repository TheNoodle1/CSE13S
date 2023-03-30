#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>

#include "trie.h"
#include "word.h"
#include "io.h"
#include "code.h"
#include "endian.h"

uint64_t total_syms = 0;
uint64_t total_bits = 0;
static uint8_t buffer[BLOCK];
static uint8_t buf[BLOCK];
static int nextbit = 0;

//
// Read up to to_read bytes from infile and store them in buf. Return the number of bytes actually
// read.
//
// Since read() may not read in as many bytes as you asked for, this function should continuously
// call read() and attempt to read as many bytes as it has not yet read. For instance, if to_read is
// 100 and the first read() call only reads 20 bytes, it should attempt to read 80 bytes the next
// time it calls read().
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int total_read = 0;
    while (total_read < to_read) {
        int num_read = read(infile, buf + total_read, to_read - total_read);
        if (num_read <= 0) {
            break;
        }
        total_read += num_read;
    }
    return total_read;
}

//
// Write up to to_write bytes from buf into outfile. Return the number of bytes actually written.
//
// Similarly to read_bytes, this function will need to call write() in a loop to ensure that it
// writes as many bytes as possible.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int total_written = 0;
    while (total_written < to_write) {
        int num_written = write(outfile, buf + total_written, to_write - total_written);
        if (num_written <= 0) {
            break;
        }
        total_written += num_written;
    }
    return total_written;
}

//
// Read a file header from infile into *header.
//
// This function need not create any additional buffer to store the contents of the file header.
// File headers, like any struct or any value in C, are just represented by bytes in memory which
// means you can use read_bytes to read however many bytes the header consumes (use sizeof!) into
// *header.
//
// Since we decided that the canonical byte order for our headers is little-endian, this function
// will need to swap the byte order of both the header fields if it is run on a big-endian system.
// For example, here is how the 4 bytes of the magic number will look when written to the file:
//
// +------+------+------+------+
// | 0xAC | 0xBA | 0xAD | 0xBA |
// +------+------+------+------+
//
// A big-endian computer would interpret those bytes as the 4-byte number 0xACBAADBA, which is
// not what we want, so you would have to change the order of those bytes in memory. A little-endian
// computer will interpret that as 0xBAADBAAC.
//
// This function should also make sure the magic number is correct. Since it has no return value you
// may call assert() to do that, or print out an error message and exit the program, or use some
// other way to report the error.
//
void read_header(int infile, FileHeader *header) {
    // Read in sizeof(FileHeader) bytes from the input file into the header
    if (read(infile, header, sizeof(FileHeader)) != sizeof(FileHeader)) {
        // Handle error
    }

    // Swap endianness of the magic field if necessary
    if (big_endian()) {
        header->magic = swap32(header->magic);
    }

    // Verify the magic number
    if (header->magic != 0xBAADBAAC) {
        // Handle error
    }
}

//
// Write a file header from *header to outfile. Like above, this function should swap the byte order
// of the header's two fields if necessary.
//
void write_header(int outfile, FileHeader *header) {
    // Swap endianness of the fields if necessary
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }

    // Write sizeof(FileHeader) bytes to the output file from the header
    if (write(outfile, header, sizeof(FileHeader)) != sizeof(FileHeader)) {
        // Handle error
    }
}

//
// Read one symbol from infile into *sym. Return true if a symbol was successfully read, false
// otherwise.
//
// Reading one symbol at a time is slow, so this function will need to maintain a global buffer
// (an array) of BLOCK bytes. Most calls will only need to read a symbol out of that buffer, and
// then update some counter so that the function knows what position in the buffer it is at. If
// there are no more bytes in the buffer for it to return, it will have to call read_bytes to refill
// the buffer with fresh data. If this call fails then you cannot read a symbol and should return
// false.
//
bool read_sym(int infile, uint8_t *sym) {
    static int buf_pos = 0;
    static int buf_len = 0;

    if (buf_pos >= buf_len) {
        buf_len = read_bytes(infile, buf, BLOCK);
        buf_pos = 0;
        if (buf_len <= 0) {
            return false;
        }
    }
    *sym = buf[buf_pos++];
    total_syms++;
    return true;
}

//
// Write a pair -- bitlen bits of code, followed by all 8 bits of sym -- to outfile.
//
// This function should also use a buffer. It writes into individual bits in the buffer, starting
// with the least significant bit of the first byte, until the most significant bit of the first
// byte, and then the least significant bit of the second byte, and so on. You will need bitwise
// arithmetic to manipulate individual bits within your buffer -- consult asgn3/set.c if you don't
// remember how to do this.
//
// The first bit of code to be written is the least significant bit, and the same holds for sym.
//
// This function will need to track which *bit* in the buffer will be written to next. If it ever
// reaches the end of the buffer it needs to write out the contents of the buffer to outfile; you
// may use flush_pairs to do this.
//
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    // Shift the code and add it to the buffer
    for (int i = 0; i < bitlen; i++) {
        int bit = (code >> i) & 1;
        buffer[nextbit >> 3] |= (bit << (nextbit & 7));
        nextbit++;
        if (nextbit == BLOCK * 8) {
            // Buffer is full, write it out
            write(outfile, buffer, BLOCK);
            memset(buffer, 0, BLOCK);
            nextbit = 0;
        }
    }

    // Shift the symbol and add it to the buffer
    for (int i = 0; i < 8; i++) {
        int bit = (sym >> i) & 1;
        buffer[nextbit >> 3] |= (bit << (nextbit & 7));
        nextbit++;
        if (nextbit == BLOCK * 8) {
            // Buffer is full, write it out
            write(outfile, buffer, BLOCK);
            memset(buffer, 0, BLOCK);
            nextbit = 0;
        }
    }
}

//
// Write any pairs that are in write_pair's buffer but haven't been written yet to outfile.
//
// This function will need to be called at the end of encode since otherwise those pairs would never
// be written. You don't have to, but it would be easy to make this function also work when called
// by write_pair, since otherwise you would write essentially the same code in two places.
//
// If not all bits of the last byte in your buffer have been written to, make sure that the
// unwritten bits are set to zero. An easy way to do this is by zeroing the entire buffer after
// flushing it every time.
//
void flush_pairs(int outfile) {
    if (nextbit > 0) {
        int nbytes = (nextbit + 7) / 8;
        write(outfile, buffer, nbytes);
        memset(buffer, 0, BLOCK);
        nextbit = 0;
    }
}

//
// Read bitlen bits of a code into *code, and then a full 8-bit symbol into *sym, from infile.
// Return true if the complete pair was read and false otherwise.
//
// Like write_pair, this function must read the least significant bit of each input byte first, and
// will store those bits into the LSB of *code and of *sym first.
//
// It may be useful to write a helper function that reads a single bit from a file using a buffer.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    static int index = 0;
    static int bits_read = 0;
    static int nbytes = 0;

    if (bits_read >= nbytes * 8) {
        nbytes = read_bytes(infile, buf, BLOCK);
        index = 0;
        bits_read = 0;
        if (nbytes <= 0) {
            return false;
        }
    }

    *code = 0;
    int bits_left = bitlen;
    while (bits_left > 0) {
        if (bits_read >= nbytes * 8) {
            nbytes = read_bytes(infile, buf, BLOCK);
            index = 0;
            bits_read = 0;
            if (nbytes <= 0) {
                return false;
            }
        }
        int bits_to_read = bits_left;
        if (bits_to_read > 8 - (index % 8)) {
            bits_to_read = 8 - (index % 8);
        }
        *code |= ((uint16_t) (buf[index / 8] >> (index % 8)) & ((1 << bits_to_read) - 1))
                 << (bitlen - bits_left);
        index += bits_to_read;
        bits_left -= bits_to_read;
        bits_read += bits_to_read;
    }

    *sym = 0;
    bits_left = 8;
    while (bits_left > 0) {
        if (bits_read >= nbytes * 8) {
            nbytes = read_bytes(infile, buf, BLOCK);
            index = 0;
            bits_read = 0;
            if (nbytes <= 0) {
                return false;
            }
        }
        int bits_to_read = bits_left;
        if (bits_to_read > 8 - (index % 8)) {
            bits_to_read = 8 - (index % 8);
        }
        *sym |= (buf[index / 8] >> (index % 8)) & ((1 << bits_to_read) - 1) << (8 - bits_left);
        index += bits_to_read;
        bits_left -= bits_to_read;
        bits_read += bits_to_read;
    }

    return (*code != STOP_CODE);
}

//
// Write every symbol from w into outfile.
//
// These symbols should also be buffered and the buffer flushed whenever necessary (note you will
// likely sometimes fill up your buffer in the middle of writing a word, so you cannot only check
// that the buffer is full at the end of this function).
//
void write_word(int outfile, Word *w) {
    int buffer_pos = 0; // current position in the buffer
    uint32_t i;
    for (i = 0; i < w->len; i++) { // loop through all symbols in the word
        buffer[buffer_pos++] = w->syms[i]; // add the symbol to the buffer
        if (buffer_pos == BLOCK) { // if buffer is full, flush it to the outfile
            if (write_bytes(outfile, buffer, buffer_pos)
                != buffer_pos) { // write may return less than buffer_pos, handle the case
                fprintf(stderr, "Error writing to outfile\n");
                exit(EXIT_FAILURE);
            }
            buffer_pos = 0; // reset buffer position to start over
        }
    }
    if (buffer_pos > 0) { // if buffer is not empty, write its contents to the outfile
        if (write_bytes(outfile, buffer, buffer_pos)
            != buffer_pos) { // write may return less than buffer_pos, handle the case
            fprintf(stderr, "Error writing to outfile\n");
            exit(EXIT_FAILURE);
        }
    }
}

//
// Write any unwritten word symbols from the buffer used by write_word to outfile.
//
// Similarly to flush_pairs, this function must be called at the end of decode since otherwise you
// would have symbols remaining in the buffer that were never written.
//
void flush_words(int outfile) {
    int buffer_pos = 0; // current position in the buffer
    if (buffer_pos > 0) { // if buffer is not empty, write its contents to the outfile
        if (write(outfile, buffer, buffer_pos)
            != buffer_pos) { // write may return less than buffer_pos, handle the case
            fprintf(stderr, "Error writing to outfile\n");
            exit(EXIT_FAILURE);
        }
        buffer_pos = 0; // reset buffer position to start over
    }
}
