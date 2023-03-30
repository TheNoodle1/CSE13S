#include "mathlib.h"

#include <stdio.h>

/* Variable to store the number of iterations executed */
static int iters = 0;

/* Function to calculate pi using Madhava's formula */
double pi_madhava(void) {
    /* Sign of the current term in the series */
    int sign = 1;
    /* Numerator of the current term in the series */
    int x = 1;
    /* Denominator of the current term in the series */
    int y = 0;
    /* Resulting sum of the series */
    double series = 0;
    /* Power of 3 for the denominator */
    long long power = 1;
    /* Store previous and current terms for comparison */
    double prev_term = 0, curr_term = 0;
    /* Difference between current and previous terms */
    double diff;

    /* Loop until the difference between current and previous terms is smaller than EPSILON */
    do {
        /* Store the previous term */
        prev_term = curr_term;
        /* Calculate the current term */
        curr_term = sign / (double) (x * power);
        /* Add the current term to the series sum */
        series = series + curr_term;
        /* Calculate the difference between current and previous terms */
        diff = curr_term - prev_term;
        /* Take absolute value of the difference */
        if (diff < 0)
            diff = -diff;
        /* Update values for the next iteration */
        x = x + 2;
        y = y + 1;
        sign = sign * -1;
        power = power * 3;
        iters++;
    } while (diff >= EPSILON);

    /* Calculate pi using Madhava's formula */
    double myPi = sqrt_newton(12) * series;

    return myPi;
}

/* Function to return the number of iterations executed in pi_madhava */
int pi_madhava_terms(void) {
    return iters;
}
