#include "bign.h"

#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void random_range(mpz_t result, mpz_t min, mpz_t max) {
    gmp_randstate_t state;
    mpz_t range;
    
    // Initialize random state
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    
    // Calculate range
    mpz_init(range);
    mpz_sub(range, max, min);
    mpz_add_ui(range, range, 1);
    
    // Generate random number
    mpz_urandomm(result, state, range);
    
    // Add min to shift to desired range
    mpz_add(result, result, min);
    
    // Clear
    mpz_clear(range);
    gmp_randclear(state);
}

void print_mpz_hex(mpz_t n) {
    char *hex = mpz_get_str(NULL, 16, n);
    printf("0x%s\n", hex);
    free(hex);
}

