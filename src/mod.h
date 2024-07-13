#ifndef MOD_H
#define MOD_H


// Define a struct to hold the results of the Extended Euclidean Algorithm
struct Result {
    int gcd;  // Greatest common divisor
    int x;    // Coefficient of a
    int y;    // Coefficient of b
};

// Extended Euclidean Algorithm
struct Result extended_euclidean_algorithm(int a, int b);

/// calculates the modular multiplicative inverse of a mod p
int modmult_inverse(int a, int p);

#endif
