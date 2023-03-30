#include "mathlib.h"

#include <stdio.h>

static int counter = 0;

/* Function to compute pi using Viete's formula */
double pi_viete(void) {
    double pi_val = sqrt_newton(2) / 2;
    double base = sqrt_newton(2);

    /* Iterating the formula until desired precision is reached */
    while ((2 - base) > EPSILON) {
        base = sqrt_newton(2 + base);
        pi_val *= base / 2;
        counter++;
    }

    /* Returning the result */
    return 2 / pi_val;
}

/* Function to get the number of iterations required to compute pi using Viete's formula */
int pi_viete_factors(void) {
    return counter;
}
