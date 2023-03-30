#include "mathlib.h"

#include <stdio.h>

// Define a static variable to store the number of terms
static int num_terms = 0;

// Define a function to calculate the value of e
double e() {
    // Initialize x to 1
    double x = 1;
    // Initialize e to 1
    double e = 1;
    // Declare a variable for the loop index
    int i;
    // Initialize the factorial to 1
    int factorial = 1;
    // Initialize the power to 1
    int power = 1;
    // Initialize the previous term to 1
    double prev_term = 1;
    // Declare a variable for the current term
    double curr_term;

    // Loop until the change between consecutive terms becomes small enough
    for (i = 1;; i++) {
        // Increment the number of terms
        num_terms++;
        // Calculate the factorial
        factorial *= i;
        // Calculate the power
        power *= x;
        // Calculate the current term as the power divided by the factorial
        curr_term = (double) power / factorial;
        // Add the current term to the sum of terms
        e += curr_term;
        // Check if the difference between the current and the previous term is greater than or equal to EPSILON
        if ((curr_term - prev_term) >= EPSILON)
            break;
        // Store the current term as the previous term
        prev_term = curr_term;
    }
    // Return the final value of the sum of terms
    return e;
}

// Define a function to return the number of terms used in the calculation
int e_terms() {
    return num_terms;
}
