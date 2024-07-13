#ifndef CRYPTO_H
#define CRYPTO_H

#include <gmp.h>

void mpz_to_bytes(mpz_t n, unsigned char *bytes, size_t byte_count); 

void bytes_to_mpz(mpz_t n, const unsigned char *bytes, size_t byte_count); 


void mpz_sha256(mpz_t input, mpz_t output); 
void mpz_ripemd160(mpz_t input, mpz_t output);


#endif
