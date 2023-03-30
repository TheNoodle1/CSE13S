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

void print_help(void);
int bit_length(uint16_t n);

int main(int argc, char *argv[]) {
    int opt;
    int infile = STDIN_FILENO; // Default input
    int outfile = STDOUT_FILENO; // Default output
    bool verbose = false;
    setlocale(LC_ALL, "");

    while ((opt = getopt(argc, argv, "vh i: o:")) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'i':
            infile = open(optarg, O_RDONLY);
            if (infile == -1) {
                perror("Failed to open input file");
                return 1;
            }
            break;
        case 'o':
            outfile = open(optarg, O_CREAT | O_WRONLY | O_TRUNC, 0666);
            if (outfile == -1) {
                perror("Failed to open output file");
                return 1;
            }
            break;
        case 'h': print_help(); return 0;
        default: print_help(); return 1;
        }
    }

    int compressed_size = 0;
    int uncompressed_size = 0;
    float compression_ratio = 0.0;

    struct stat stats;
    Word *w = malloc(sizeof(Word)); // change to allocate memory for Word
    fstat(outfile, &stats);
    w->syms = NULL; // initialize syms to NULL
    w->len = stats.st_size; // update len to file size

    WordTable *table = wt_create();
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;

    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code)) == true) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        // uncompressed_size += table[next_code]->len;
        // compressed_size += bit_length(next_code) / 8;
        next_code++;
        if ((next_code == MAX_CODE) == true) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    flush_words(outfile);
    wt_delete(table);

    compression_ratio = (100.0 * (1.0 - (compressed_size / uncompressed_size)));

    if (verbose == true) {
        printf("Compressed file size: %d bytes\n", compressed_size);
        printf("Uncompressed file size: %d bytes\n", uncompressed_size);
        printf("Compression ratio: %2.2f%%\n", compression_ratio);
    }

    close(infile);
    close(outfile);

    return 0;
}

void print_help(void) {
    printf("SYNOPSIS\n");
    printf("   Decompresses files with the LZ78 decompression algorithm.\n");
    printf("   Used with files compressed with the corresponding encoder.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./decode [-vh] [-i input] [-o output]");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -v          Display decompression statistics\n");
    printf("   -i input    Specify input to decompress (stdin by default)\n");
    printf("   -o output   Specify output of decompressed input (stdout by default)\n");
    printf("   -h          Display program usage\n");
}

int bit_length(uint16_t n) {
    int length = 0;
    while (n > 0) {
        length++;
        n >>= 1;
    }
    return length;
}

/*
void decompress(int infile, int outfile) {
    WordTable *table = wt_create();
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;

    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code)) == true) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        next_code++;
        if ((next_code == MAX_CODE) == true) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    flush_words(outfile);
}*/
