#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/evp.h>


void mpz_to_bytes(mpz_t n, unsigned char *bytes, size_t byte_count) {
    mpz_export(bytes, NULL, 1, 1, 1, 0, n);
}

void bytes_to_mpz(mpz_t n, const unsigned char *bytes, size_t byte_count) {
    mpz_import(n, byte_count, 1, 1, 1, 0, bytes);
}


void mpz_sha256(mpz_t input, mpz_t output) {
    unsigned char input_bytes[32];
    unsigned char output_bytes[SHA256_DIGEST_LENGTH];
    
    mpz_to_bytes(input, input_bytes, 32);

    // SHA256_CTX sha256_ctx;
    // SHA256_Init(&sha256_ctx);
    // SHA256_Update(&sha256_ctx, input_bytes, 32);
    // SHA256_Final(output_bytes, &sha256_ctx);
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        fprintf(stderr, "Failed to create EVP_MD_CTX\n");
        return;
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (EVP_DigestUpdate(mdctx, input_bytes, 32) != 1) {
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (EVP_DigestFinal_ex(mdctx, output_bytes, NULL) != 1) {
        EVP_MD_CTX_free(mdctx);
        return;
    }

    EVP_MD_CTX_free(mdctx);

    bytes_to_mpz(output, output_bytes, SHA256_DIGEST_LENGTH);
}

void mpz_ripemd160(mpz_t input, mpz_t output) {
    unsigned char input_bytes[32];
    unsigned char output_bytes[RIPEMD160_DIGEST_LENGTH];
    
    mpz_to_bytes(input, input_bytes, 32);

    RIPEMD160_CTX ripemd160_ctx;
    // RIPEMD160_Init(&ripemd160_ctx);
    // RIPEMD160_Update(&ripemd160_ctx, input_bytes, 32);
    // RIPEMD160_Final(output_bytes, &ripemd160_ctx);

    bytes_to_mpz(output, output_bytes, RIPEMD160_DIGEST_LENGTH);
}


