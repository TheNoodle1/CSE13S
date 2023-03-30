#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

// Print usage information.
void print_help(void) {
    printf("SYNOPSIS\n");
    printf("   Decrypts data using SS decryption.\n");
    printf("   Encrypted data is encrypted by the encrypt program.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./decrypt [OPTIONS]");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -i infile       Input file of data to decrypt (default: stdin).\n");
    printf("   -o outfile      Output file for decrypted data (default: stdout).\n");
    printf("   -n pvfile       Private key file (default: ss.priv).\n");
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    int opt;
    FILE *infile = stdin; // Default value
    FILE *outfile = stdout; // Default value
    char *pvfile = "ss.priv";
    bool verbose = false;

    while ((opt = getopt(argc, argv, "hvi:o:n:")) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (!infile) {
                perror("Failed to open input file");
                return 1;
            }
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (!outfile) {
                perror("Failed to open output file");
                fclose(infile);
                return 1;
            }
            break;
        case 'n': pvfile = optarg; break;
        case 'v': verbose = true; break;
        case 'h':
            print_help();
            fclose(infile);
            fclose(outfile);
            return 0;
        default:
            print_help();
            fclose(infile);
            fclose(outfile);
            return 1;
        }
    }

    // Open private key file.
    FILE *pvfp = fopen(pvfile, "r");
    if (!pvfp) {
        perror("Failed to open private key file");
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    // Read private key from file.
    mpz_t pq, d;
    mpz_init(pq);
    mpz_init(d);
    ss_read_priv(pq, d, pvfp);
    if (mpz_cmp_ui(pq, 0) == 0 || mpz_cmp_ui(d, 0) == 0) {
        perror("Failed to read private key from file");
        fclose(pvfp);
        fclose(infile);
        fclose(outfile);
        mpz_clear(pq);
        mpz_clear(d);
        return 1;
    }

    // Print private key if verbose output is enabled.
    if (verbose) {
        size_t pq_bits = mpz_sizeinbase(pq, 2);
        gmp_printf("pq (%zu bits) = ", pq_bits);
        mpz_out_str(stdout, 10, pq);
        gmp_printf("\n");

        size_t d_bits = mpz_sizeinbase(d, 2);
        gmp_printf("d  (%zu bits) = ", d_bits);
        mpz_out_str(stdout, 10, d);
        gmp_printf("\n");
    }

    // Decrypt the file.
    ss_decrypt_file(infile, outfile, d, pq);

    // Close private key file and clear variables.
    fclose(pvfp);
    fclose(infile);
    fclose(outfile);
    mpz_clear(pq);
    mpz_clear(d);

    return 0;
}
