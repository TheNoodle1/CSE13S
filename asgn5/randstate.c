#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <stdlib.h>

#include "randstate.h"

// global random state variable
gmp_randstate_t state;

void randstate_init(uint64_t seed) {
    // initialize the global random state with Mersenne Twister algorithm
    gmp_randinit_mt(state);

    // seed the random state with the given seed
    gmp_randseed_ui(state, seed);

    // set the seed for the m random number generator
    srandom(seed);
}

void randstate_clear(void) {
    // clear and free memory used by the random state
    gmp_randclear(state);
}
