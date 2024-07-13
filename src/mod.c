#include "mod.h"


// Extended Euclidean Algorithm
struct Result extended_euclidean_algorithm(int a, int b) {
    int old_r = a; 
    int r = b;

    int old_s = 1;
    int s = 0;

    int old_t = 0;
    int t = 1;

    while (r != 0) {
        int quotient = old_r / r;

        int temp_r = r;
        r = old_r - quotient * r;
        old_r = temp_r;

        int temp_s = s;
        s = old_s - quotient * s;
        old_s = temp_s;

        int temp_t = t;
        t = old_t - quotient * t;
        old_t = temp_t;
    }

    struct Result result = {old_r, old_s, old_t};
    return result;
}

/// calculates the modular multiplicative inverse of a mod p
int modmult_inverse(int a, int p) {
    struct Result result = extended_euclidean_algorithm(a, p);
    return result.x % p;
}

