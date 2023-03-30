#include "mathlib.h"

#include <stdio.h>

// Define a static variable to store the number of iterations
static long double iterations = 0;

// Define a function to calculate the value of π using the BBP formula
double pi_bbp(void) {
    // Initialize the base value to 1
    long double base = 1;
    // Initialize the current and previous terms to 0
    long double current = 0;
    long double previous = 0;
    // Initialize the term index to 0
    int k = 0;

    // Loop until the change between consecutive terms becomes small enough
    while (1) {
        // Store the current value of the current term as previous
        previous = current;
        // Calculate the numerator for the current term
        long double numerator = k * (120 * k + 151) + 47;
        // Calculate the denominator for the current term
        long double denominator = k * (k * (k * (512 * k + 1024) + 712) + 194) + 15;
        // Calculate the current term as the numerator divided by the denominator and multiplied by the base value
        iterations = numerator / denominator * base;
        // Add the current term to the sum of terms
        current += iterations;
        // Divide the base value by 16
        base /= 16;
        // Increment the term index
        k++;
        // Store the current value of the term index as the number of iterations
        iterations = k;

        // Check if the difference between the previous and the current term is less than EPSILON
        if (current > previous) {
            if (current - previous < EPSILON)
                break;
        } else {
            if (previous - current < EPSILON)
                break;
        }
    }
    // Return the final value of the sum of terms
    return current;
}

// Define a function to return the number of terms used in the calculation
int pi_bbp_terms(void) {
    return iterations;
}
