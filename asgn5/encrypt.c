#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

// Print usage information.
void print_help(void) {
    printf("SYNOPSIS\n");
    printf("   Encrypts data using SS encryption.\n");
    printf("   Encrypted data is decrypted by the decrypt program.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./encrypt [OPTIONS]");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
    printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
    printf("   -n pbfile       Public key file (default: ss.pub).\n");
}

int main(int argc, char *argv[]) {
    int opt;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pbfile = "ss.pub";
    bool verbose = false;

    while ((opt = getopt(argc, argv, "hv i: o: n:")) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (!infile) {
                printf("Error: could not open input file %s\n", optarg);
                return 1;
            }
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (!outfile) {
                printf("Error: could not open output file %s\n", optarg);
                return 1;
            }
            break;
        case 'n': pbfile = optarg; break;
        case 'v': verbose = true; break;
        case 'h': print_help(); return 0;
        default: print_help(); return 1;
        }
    }

    // Open public key file
    FILE *f = fopen(pbfile, "r");
    if (!f) {
        printf("Error: could not open public key file %s\n", pbfile);
        return 1;
    }

    // Read the public key from the opened public key file
    mpz_t n;
    mpz_init(n);

    char hex_key[512];
    if (fgets(hex_key, 512, f) == NULL) {
        printf("Error: invalid public key file format\n");
        return 1;
    }

    if (mpz_set_str(n, hex_key, 16) == -1) {
        printf("Error: invalid public key file format\n");
        return 1;
    }

    char username[128];
    if (fgets(username, 128, f) == NULL) {
        printf("Error: invalid public key file format\n");
        return 1;
    }
    username[strcspn(username, "\n")] = 0;

    // Print verbose output if enabled
    if (verbose) {
        gmp_printf("user = %s\n", username);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    // Encrypt input file
    ss_encrypt_file(infile, outfile, n);

    // Clean up
    fclose(f);
    mpz_clear(n);

    return 0;
}
