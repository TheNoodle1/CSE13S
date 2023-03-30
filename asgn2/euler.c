#include "mathlib.h"

#include <stdio.h>

// Define a static variable to store the number of terms
static int terms = 0;

// Define a function to calculate the value of pi using the Euler method
double pi_euler() {
    // Initialize pi to 0
    double pi = 0;
    // Declare a variable to store the previous value of pi
    double prev_pi = 0;

    // Loop until the change between consecutive approximations becomes small enough
    for (int k = 1;; k++) {
        // Store the previous approximation of pi
        prev_pi = pi;
        // Update the approximation of pi
        pi += 1.0 / (k * k);
        // Increment the number of terms
        terms++;
        // Check if the difference between the current and the previous approximation is less than or equal to EPSILON
        if (pi - prev_pi <= EPSILON)
            break;
    }
    // Calculate x as 6 times the approximation of pi
    double x = (6 * pi);
    // Return the square root of x using Newton's method
    return sqrt_newton(x);
}

// Define a function to return the number of terms used in the calculation
int pi_euler_terms(void) {
    return terms;
}
