#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

//
// Generates the components for a new SS key.
//
// Provides:
//  p:  first prime
//  q: second prime
//  n: public modulus/exponent
//
// Requires:
//  nbits: minimum # of bits in n
//  iters: iterations of Miller-Rabin to use for primality check
//  all mpz_t arguments to be initialized
//
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    // compute number of bits for p
    uint64_t p_bits = (nbits / 5) + random() % (nbits / 5);

    // generate p using Miller-Rabin primality test
    make_prime(p, p_bits, iters);

    // compute q = n / (p^2)
    mpz_t p_square;
    mpz_init(p_square);
    mpz_mul(p_square, p, p);
    mpz_div(q, n, p_square);

    // compute the number of bits for q
    uint64_t q_bits = nbits - 2 * p_bits;

    // generate a prime number for q using Miller-Rabin primality test
    make_prime(q, q_bits, iters);

    // compute n = p^2 * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    // clear temporary variables
    mpz_clear(p_square);
}

//
// Generates components for a new SS private key.
//
// Provides:
//  d:  private exponent
//  pq: private modulus
//
// Requires:
//  p:  first prime number
//  q: second prime number
//  all mpz_t arguments to be initialized
//
void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t p_minus_1, q_minus_1, lambda, n, gcd_result;
    mpz_inits(p_minus_1, q_minus_1, lambda, n, gcd_result, NULL);

    // Compute p-1 and q-1
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);

    // Compute gcd(p-1, q-1)
    gcd(gcd_result, p_minus_1, q_minus_1);

    // Compute lambda(pq) = lcm(p-1, q-1) = (p-1) * (q-1) / gcd_result(p-1, q-1)
    mpz_mul(lambda, p_minus_1, q_minus_1);
    mpz_div(lambda, lambda, gcd_result);

    // Compute n = p^2 * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    mpz_mul(pq, p, q);

    // Compute the inverse of n modulo lambda(pq)
    mod_inverse(d, n, lambda);

    // Clean up
    mpz_clears(p_minus_1, q_minus_1, lambda, n, gcd_result, NULL);
}

//
// Export SS public key to output stream
//
// Requires:
//  n: public modulus/exponent
//  username: login name of keyholder ($USER)
//  pbfile: open and writable file stream
//

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    // write n as hexstring to pbfile, followed by newline
    mpz_out_str(pbfile, 16, n);
    fprintf(pbfile, "\n");

    // write username to pbfile, followed by newline
    fprintf(pbfile, "%s\n", username);
}

//
// Export SS private key to output stream
//
// Requires:
//  pq: private modulus
//  d:  private exponent
//  pvfile: open and writable file stream
//
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    // write pq as a hex string with trailing newline
    gmp_fprintf(pvfile, "%Zx\n", pq);

    // write d as a hex string with trailing newline
    gmp_fprintf(pvfile, "%Zx\n", d);
}

//
// Import SS public key from input stream
//
// Provides:
//  n: public modulus
//  username: $USER of the pubkey creator
//
// Requires:
//  pbfile: open and readable file stream
//  username: requires sufficient space
//  all mpz_t arguments to be initialized
//
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    char *n_hexstring = NULL;
    size_t n_hexstring_size = 0;

    // Read the hexstring representation of n from the input stream
    ssize_t read = getline(&n_hexstring, &n_hexstring_size, pbfile);
    if (read == -1) {
        fprintf(stderr, "Error reading public key from input stream\n");
        exit(1);
    }
    mpz_set_str(n, n_hexstring, 16);

    // Read the username from the input stream
    read = getline(&username, &n_hexstring_size, pbfile);
    if (read == -1) {
        fprintf(stderr, "Error reading username from input stream\n");
        exit(1);
    }

    // Remove trailing newline character from username
    username[strcspn(username, "\n")] = '\0';

    // Free memory allocated by getline for the n_hexstring buffer
    free(n_hexstring);
}

//
// Import SS private key from input stream
//
// Provides:
//  pq: private modulus
//  d:  private exponent
//
// Requires:
//  pvfile: open and readable file stream
//  all mpz_t arguments to be initialized
//
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    char buf[256];
    fgets(buf, sizeof(buf), pvfile);
    mpz_set_str(pq, buf, 16);
    fgets(buf, sizeof(buf), pvfile);
    mpz_set_str(d, buf, 16);
}

//
// Encrypt number m into number c
//
// Provides:
//  c: encrypted integer
//
// Requires:
//  m: original integer
//  n: public exponent/modulus
//  all mpz_t arguments to be initialized
//
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

//
// Encrypt an arbitrary file
//
// Provides:
//  fills outfile with the encrypted contents of infile
//
// Requires:
//  infile: open and readable file stream
//  outfile: open and writable file stream
//  n: public exponent and modulus
//
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    // Calculate block size k
    int b = 256; // Set the number of bits per byte
    int log2p = mpz_sizeinbase(n, 2);
    int k = ((b * (log2p - 1)) / 8);

    // Allocate memory for block
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));
    if (block == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for block.\n");
        exit(EXIT_FAILURE);
    }

    // Encrypt data in blocks
    size_t j;
    while ((j = fread(block + 1, sizeof(uint8_t), k - 1, infile)) > 0) {
        // Set first byte of block to 0xFF
        block[0] = 0xFF;

        // Initialize and set m to the input message
        mpz_t m;
        mpz_init(m);
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, block);

        // Check if block is equal to 0 or 1
        if (mpz_cmp_ui(m, 0) == 0 || mpz_cmp_ui(m, 1) == 0) {
            fprintf(stderr, "Error: Block value cannot be 0 or 1.\n");
            exit(EXIT_FAILURE);
        }

        // Encrypt block with ss_encrypt
        mpz_t c;
        mpz_init(c);
        ss_encrypt(c, m, n);

        // Write encrypted block to outfile as hexstring
        char *hexstr = mpz_get_str(NULL, 16, c);
        fprintf(outfile, "%s\n", hexstr);
        free(hexstr);

        mpz_clear(m);
        mpz_clear(c);
    }

    free(block);
}

//
// Decrypt number c into number m
//
// Provides:
//  m: decrypted/original integer
//
// Requires:
//  c: encrypted integer
//  d: private exponent
//  pq: private modulus
//  all mpz_t arguments to be initialized
//
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

//
// Decrypt a file back into its original form.
//
// Provides:
//  fills outfile with the unencrypted data from infile
//
// Requires:
//  infile: open and readable file stream to encrypted data
//  outfile: open and writable file stream
//  d: private exponent
//  pq: private modulus
//
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    // Calculate k
    int b = 8;
    int c = 2;
    int k = (mpz_sizeinbase(pq, 2) - 1) / (b * c);

    // Allocate memory for block
    uint8_t *block = malloc(k);

    // Initialize temporary variables
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Iterate over lines in infile
    while ((read = getline(&line, &len, infile)) != -1) {
        // Convert hex string to mpz_t
        mpz_t c;
        mpz_init(c);
        mpz_set_str(c, line, 16);

        // Decrypt using RSA
        mpz_t m;
        mpz_init(m);
        mpz_powm(m, c, d, pq);

        // Convert decrypted value to bytes
        // Export m to block
        size_t block_size;
        mpz_export(&block[0], &block_size, 1, 1, 0, 0, m);

        // Write bytes from block to outfile, but skip the first byte
        fwrite(block + 1, 1, block_size - 1, outfile);

        // Clear temporary variables
        mpz_clear(c);
        mpz_clear(m);
    }

    // Free memory
    free(block);
    free(line);
}
