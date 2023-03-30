#include "mathlib.h"

#include <stdio.h>

// A static integer variable to keep track of the number of iterations
static int iterationCount = 0;

// Function to compute the square root of a number using the Newton-Raphson method
double sqrt_newton(double x) {
    // Initial approximation of the square root
    double approx = x / 2;
    // A variable to store the previous approximation
    double prev_approx;

    // Loop until the difference between the previous and current approximations is less than a threshold
    do {
        // Save the current approximation to prev_approx
        prev_approx = approx;
        // Update the approximation using the Newton-Raphson formula
        approx = (approx + x / approx) / 2;
        // Increment the iteration count
        iterationCount++;
    } while (x - prev_approx * prev_approx >= 0.00001 || prev_approx * prev_approx - x >= 0.00001);

    // Return the final approximation
    return approx;
}

// Function to return the number of iterations in the sqrt_newton function
int sqrt_newton_iters(void) {
    return iterationCount;
}
