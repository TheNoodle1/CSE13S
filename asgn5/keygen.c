#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#define DEFAULT_ITERATIONS   50
#define DEFAULT_PUBKEY_FILE  "ss.pub"
#define DEFAULT_PRIVKEY_FILE "ss.priv"
#define PERM_PRIVATE_KEY     0600
#define MIN_BITS             256

// Print usage information.
void print_help(void) {
    printf("SYNOPSIS\n");
    printf("   Generates an SS public/private key pair.\n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./keygen [OPTIONS]");
    printf("\n");
    printf("OPTIONS");
    printf("   -h              Display program help and usage.\n");
    printf("   -v              Display verbose program output.\n");
    printf("   -b bits         Minimum bits needed for public key n (default: 256).\n");
    printf("   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n");
    printf("   -n pbfile       Public key file (default: ss.pub).\n");
    printf("   -d pvfile       Private key file (default: ss.priv).\n");
    printf("   -s seed         Random seed for testing.\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int bits = MIN_BITS;
    int iters = DEFAULT_ITERATIONS;
    char *pubkeyfile = DEFAULT_PUBKEY_FILE;
    char *privkeyfile = DEFAULT_PRIVKEY_FILE;
    unsigned long seed = time(NULL);
    bool verbose = false;

    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b': bits = atoi(optarg); break;
        case 'i': iters = atoi(optarg); break;
        case 'n': pubkeyfile = optarg; break;
        case 'd': privkeyfile = optarg; break;
        case 's': seed = strtoul(optarg, NULL, 0); break;
        case 'v': verbose = true; break;
        case 'h': print_help(); return 0;
        default: print_help(); return 1;
        }
    }

    // Open the public and private key files.
    FILE *pubkey = fopen(pubkeyfile, "w");
    if (pubkey == NULL) {
        printf("Error: could not open public key file %s\n", pubkeyfile);
        return 1;
    }
    FILE *privkey = fopen(privkeyfile, "w");
    if (privkey == NULL) {
        printf("Error: could not open private key file %s\n", privkeyfile);
        fclose(pubkey);
        return 1;
    }

    // Set private key file permissions to 0600
    if (fchmod(fileno(privkey), PERM_PRIVATE_KEY) != 0) {
        printf("Error: could not set private key file permissions\n");
        fclose(pubkey);
        fclose(privkey);
        return 1;
    }

    // Get user's name.
    char *username = getenv("USER");
    if (username == NULL) {
        fprintf(stderr, "Error: could not get current user's name\n");
        return 1;
    }

    // Initialize random state.
    randstate_init(seed);

    // Generate public and private keys.
    mpz_t p, q, n, e, d, pq;
    mpz_inits(p, q, n, e, d, pq, NULL);

    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, pq, p, q);

    // Writing the computed public and private key to their respective files.
    ss_write_pub(n, username, pubkey);
    ss_write_priv(pq, d, privkey);

    // Print verbose if -v is enabled.
    if (verbose == true) {
        printf("user = %s\n", username);
        printf("p  (%lu bits) = %s\n", mpz_sizeinbase(p, 2), mpz_get_str(NULL, 10, p));
        printf("q  (%lu bits) = %s\n", mpz_sizeinbase(q, 2), mpz_get_str(NULL, 10, q));
        printf("n  (%lu bits) = %s\n", mpz_sizeinbase(n, 2), mpz_get_str(NULL, 10, n));
        printf("pq (%lu bits) = %s\n", mpz_sizeinbase(pq, 2), mpz_get_str(NULL, 10, pq));
        printf("d  (%lu bits) = %s\n", mpz_sizeinbase(d, 2), mpz_get_str(NULL, 10, d));
    }

    // Close the public and private key files, clear the random state with randstate_clear(), and clear any mpz_t variables used.
    fclose(pubkey);
    fclose(privkey);
    randstate_clear();
    mpz_clears(p, q, n, d, pq, NULL);

    return 0;
}
