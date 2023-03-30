#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

// This function calculates the greatest common divisor of two mpz_t integers using the
// Euclidean algorithm.
void gcd(mpz_t g, const mpz_t a, const mpz_t b) {

    // Initialize temporary variables
    mpz_t t, a_copy, b_copy;
    mpz_inits(t, a_copy, b_copy, NULL);

    // Make copies of the input variables
    mpz_set(a_copy, a);
    mpz_set(b_copy, b);

    // While b is not 0, calculate the remainder of a / b and update variables
    while (mpz_cmp_ui(b_copy, 0) != 0) {
        mpz_mod(t, a_copy, b_copy);
        mpz_set(a_copy, b_copy);
        mpz_set(b_copy, t);
    }

    // Set the output variable to a_copy, which holds the GCD
    mpz_set(g, a_copy);

    // Clear temporary variables
    mpz_clears(t, a_copy, b_copy, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, r1, t, t1, q, tmp;

    // Initialize variables
    mpz_init_set(r, n);
    mpz_init_set(r1, a);
    mpz_init_set_ui(t, 0);
    mpz_init_set_ui(t1, 1);
    mpz_init(q);
    mpz_init(tmp);

    // Extended Euclidean Algorithm
    while (mpz_cmp_ui(r1, 0) != 0) {
        // Calculate quotient q and update r and r1
        mpz_div(q, r, r1);
        mpz_set(tmp, r);
        mpz_set(r, r1);
        mpz_mul(r1, q, r1);
        mpz_sub(r1, tmp, r1);

        // Update t and t1
        mpz_set(tmp, t);
        mpz_set(t, t1);
        mpz_mul(q, q, t1);
        mpz_sub(t1, tmp, q);
    }

    // Check if a modular inverse exists
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0); // Set o to 0 if inverse does not exist
    } else {
        // Compute modular inverse of a modulo n
        if (mpz_cmp_ui(t, 0) < 0) {
            mpz_add(t, t, n);
        }
        mpz_set(o, t);
    }

    // Clear variables
    mpz_clear(r);
    mpz_clear(r1);
    mpz_clear(t);
    mpz_clear(t1);
    mpz_clear(q);
    mpz_clear(tmp);
}

void pow_mod(mpz_t out, const mpz_t base, const mpz_t exponent, const mpz_t modulus) {
    mpz_t v, p, d;
    mpz_inits(v, p, d, NULL);

    // Initialize v to 1 and p to the base.
    mpz_set_ui(v, 1);
    mpz_set(p, base);

    // Copy the exponent to a non-const variable.
    mpz_set(d, exponent);

    // Perform exponentiation by squaring.
    while (mpz_cmp_ui(d, 0) > 0) {
        if (mpz_odd_p(d)) {
            // Multiply v by p and reduce modulo modulus.
            mpz_mul(v, v, p);
            mpz_mod(v, v, modulus);
        }
        // Square p and reduce modulo modulus.
        mpz_mul(p, p, p);
        mpz_mod(p, p, modulus);
        // Divide the exponent by 2 (i.e. shift it right by 1).
        mpz_tdiv_q_2exp(d, d, 1);
    }

    // Set the output variable to v.
    mpz_set(out, v);

    // Clean up the temporary variables.
    mpz_clears(v, p, d, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    mpz_t r, y, n_minus_one, two;
    mpz_inits(r, y, n_minus_one, two, NULL);
    mpz_set_ui(two, 2);

    // ensure that n > 1
    if (mpz_cmp_ui(n, 1) <= 0) {
        return false;
    }

    // check if n is 2 or 3
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    }

    // write n-1 = 2^s * r such that r is odd
    mpz_sub_ui(n_minus_one, n, 1);
    mpz_set(r, n_minus_one);
    unsigned long int s = 0;
    while (mpz_even_p(r)) {
        mpz_divexact_ui(r, r, 2);
        s++;
    }

    // do Miller-Rabin test with iters number of iterations
    for (uint64_t i = 0; i < iters; i++) {
        // choose random a in [2, n-2]
        mpz_t a;
        mpz_init(a);
        mpz_urandomm(a, state, n_minus_one);
        mpz_add_ui(a, a, 2);

        // calculate y = a^r mod n
        pow_mod(y, a, r, n);

        if (mpz_cmp_ui(y, 1) == 0 || mpz_cmp(y, n_minus_one) == 0) {
            // inconclusive result, continue to next iteration
            mpz_clear(a);
            continue;
        }

        bool is_composite = true;
        for (unsigned long int j = 1; j < s; j++) {
            pow_mod(y, y, two, n);
            if (mpz_cmp_ui(y, 1) == 0) {
                // n is composite, y = 1, and we've found a nontrivial square root of 1 modulo n
                mpz_clear(a);
                return false;
            } else if (mpz_cmp(y, n_minus_one) == 0) {
                // inconclusive result, continue to next iteration
                is_composite = false;
                break;
            }
        }

        if (is_composite) {
            // n is composite, we've found a nontrivial square root of 1 modulo n
            mpz_clear(a);
            return false;
        }

        mpz_clear(a);
    }

    // n is probably prime
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    // Initialize a temporary variable to hold the candidate prime
    mpz_t candidate;
    mpz_init(candidate);

    // Generate a candidate prime number of the specified number of bits
    do {
        // Set the candidate prime number to a random integer of the specified number of bits
        mpz_urandomb(candidate, state, bits);

        // Make sure the candidate prime number has the specified number of bits
        mpz_setbit(candidate, bits);

        // Make sure the candidate prime number is odd
        mpz_setbit(candidate, 0);
    } while (!is_prime(candidate, iters));

    // Copy the candidate prime number into the output variable
    mpz_set(p, candidate);

    // Clear the temporary variable
    mpz_clear(candidate);
}
