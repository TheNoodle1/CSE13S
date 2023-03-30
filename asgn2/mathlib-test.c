#include "mathlib.h" // Include the math library header file

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define PI "M_PI"
#define E  "M_E"

int main(int argc, char **argv) {
    int opt;
    int sflag = 0; // Flag to enable statistics printing

    bool e_option = false;
    bool b_option = false;
    bool m_option = false;
    bool v_option = false;
    bool r_option = false;
    bool n_option = false;

    // Use getopt to parse the command-line options
    while ((opt = getopt(argc, argv, "aebmvrns:h")) != -1) {
        switch (opt) {
        case 'a':
            // Run all tests
            e_option = true;
            b_option = true;
            m_option = true;
            v_option = true;
            r_option = true;
            n_option = true;
            sflag = 1;
            break;
        case 'e':
            // Run e approximation test
            e_option = true;
            break;
        case 'b':
            // Run Bailey-Borwein-Plouffe π approximation test
            b_option = true;
            break;
        case 'm':
            // Run Madhava π approximation test
            m_option = true;
            break;
        case 'r':
            // Run Euler sequence π approximation test
            r_option = true;
            break;
        case 'v':
            // Run Viète π approximation test
            v_option = true;
            break;
        case 'n':
            // Run Newton-Raphson square root approximation tests
            n_option = true;
            break;
        case 's':
            sflag = 1; // Enable statistics printing
            break;
        case 'h':
            // Display help message
            printf("SYNOPSIS\n");
            printf("   A test harness for the small numerical library.\n");
            printf("\nUSAGE\n");
            printf("   ./mathlib-test.c [-aebmrvnsh]\n");
            printf("\nOPTIONS\n");
            printf("  -a   Runs all tests.\n");
            printf("  -e   Runs e test.\n");
            printf("  -b   Runs BBP pi test.\n");
            printf("  -m   Runs Madhava pi test.\n");
            printf("  -r   Runs Euler pi test.\n");
            printf("  -v   Runs Viete pi test.\n");
            printf("  -n   Runs Newton square root tests.\n");
            printf("  -s   Print verbose statistics.\n");
            printf("  -h   Display program synopsis and usage.\n");
            break;
        default:
            printf("SYNOPSIS\n");
            printf("   A test harness for the small numerical library.\n");
            printf("\nUSAGE\n");
            printf("   ./mathlib-test.c [-aebmrvnsh]\n");
            printf("\nOPTIONS\n");
            printf("  -a   Runs all tests.\n");
            printf("  -e   Runs e test.\n");
            printf("  -b   Runs BBP pi test.\n");
            printf("  -m   Runs Madhava pi test.\n");
            printf("  -r   Runs Euler pi test.\n");
            printf("  -v   Runs Viete pi test.\n");
            printf("  -n   Runs Newton square root tests.\n");
            printf("  -s   Print verbose statistics.\n");
            printf("  -h   Display program synopsis and usage.\n");
            break;
        }
    }

    // Output the results of the tests
    // Use the format: "result = value, M_value = value, diff = value\n"
    // If sflag is set, also output the number of computed terms or iterations
    if (e_option == true) {
        double e_error = absolute(e() - M_E);

        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, e_error);

        if (sflag == 1) {
            printf("e_terms = %d\n", e_terms());
        }
    }

    if (b_option == true) {
        double pi_bbp_error = absolute(pi_bbp() - M_PI);

        printf("%s%16.15lf%s%16.15lf%s%16.15lf\n", "pi_bbp() = ", pi_bbp(), ", M_PI = ", M_PI,
            ", diff = ", pi_bbp_error);

        if (sflag == 1) {
            printf("pi_bbp_terms = %d\n", pi_bbp_terms());
        }
    }

    if (m_option == true) {
        double pi_madhava_error = absolute(pi_madhava() - M_PI);

        printf("%s%.15lf%s%.15lf%s%.15lf\n", "pi_madhava() = ", pi_madhava(), ", M_PI = ", M_PI,
            ", diff = ", pi_madhava_error);

        if (sflag == 1) {
            printf("pi_madhava_terms = %d\n", pi_madhava_terms());
        }
    }

    if (r_option == true) {
        double pi_euler_error = absolute(pi_euler() - M_PI);

        printf("%s%.15lf%s%.15lf%s%.15lf\n", "pi_euler() = ", pi_euler(), ", M_PI = ", M_PI,
            ", diff = ", pi_euler_error);

        if (sflag == 1) {
            printf("pi_euler_terms = %d\n", pi_euler_terms());
        }
    }

    if (v_option == true) {
        double pi_viete_error = absolute(pi_viete() - M_PI);

        printf("%s %.15lf %s %.15lf %s %.15lf\n", "pi_viete() =", pi_viete(), ", M_PI =", M_PI,
            ", diff =", pi_viete_error);

        if (sflag == 1) {
            printf("pi_viete_factors = %d\n", pi_viete_factors());
        }
    }

    if (n_option == true) {
        for (double i = 0; i < 10; i += 0.1) {
            double sqrt_newton_error = absolute(sqrt_newton(i) - sqrt(i));

            printf("sqrt_newton(%.15lf) = %.15lf, sqrt(%.15lf) = %.15lf, diff = %.15lf\n", i,
                sqrt_newton(i), i, sqrt(i), sqrt_newton_error);
        }
    }

    return 0;
}
