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

int bit_length(uint8_t n);
void print_help(void);

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

    struct stat stats;
    // fchmod(outfile, stats.st_mode);
    FileHeader file_header; // = malloc(sizeof(FileHeader));
    fstat(outfile, &stats);
    file_header.magic = MAGIC;
    file_header.protection = stats.st_mode;

    int compressed_size = 0;
    int uncompressed_size = 0;
    float compression_ratio = 0.0;

    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    uint16_t next_code = START_CODE;

    write_header(outfile, &file_header);

    while (read_sym(infile, &curr_sym) == true) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bit_length(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code++;
        }
        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
        uncompressed_size++;
    }
    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bit_length(next_code));
        next_code++;
        next_code %= MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, bit_length(next_code));
    flush_pairs(outfile);

    compressed_size = lseek(outfile, 0, SEEK_CUR);
    lseek(outfile, 0, SEEK_SET);

    if (compressed_size == -1) {
        perror("Failed to determine compressed file size");
        return 1;
    }

    trie_delete(root);

    compression_ratio = (100.0 * (1.0 - ((float) compressed_size / (float) uncompressed_size)));

    if (verbose == true) {
        printf("Compressed file size: %d bytes\n", compressed_size);
        printf("Uncompressed file size: %d bytes\n", uncompressed_size);
        printf("Compression ratio: %2.2f%%\n", compression_ratio);
    }

    close(infile);
    close(outfile);
    // free(&file_header);

    return 0;
}

void print_help(void) {
    printf("SYNOPSIS\n");
    printf("   Compresses files using the LZ78 compression algorithm.\n");
    printf("   Compressed files are decompressed with the corresponding decoder.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./encode [-vh] [-i input] [-o output]");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -v          Display compression statistics\n");
    printf("   -i input    Specify input to compress (stdin by default)\n");
    printf("   -o output   Specify output of compressed input (stdout by default)\n");
    printf("   -h          Display program help and usage\n");
}

int bit_length(uint8_t n) {
    int length = 0;
    while (n > 0) {
        length++;
        n >>= 1;
    }
    return length;
}
