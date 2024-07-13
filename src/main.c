#include <stdio.h>

#include "bign.h"
#include "point.h"
#include "ecdsa.h"
#include "crypto.h"
#include "str.h"



void random_point_on_curve_check(EllipticCurve *curve) {
    mpz_t min;
    mpz_init_set_ui(min, 1); // min = 1

    mpz_t max;
    mpz_t p_minus_1;
    mpz_init(p_minus_1);
    mpz_sub_ui(p_minus_1, curve->p, 1); // max = p - 1
    mpz_init_set(max, p_minus_1);
    

    //
    // random point
    Point random_point;
    init_point(&random_point, "random");

    mpz_t random_x;
    mpz_t random_y;
    mpz_init(random_x);
    mpz_init(random_y);

    random_range(random_x, min, max); // random x b/w 1 `min` & `max`
    random_range(random_y, min, max); // random y b/w 1 `min` & `max`

    // set point 
    set_point_hex(&random_point, mpz_get_str(NULL, 16, random_x), mpz_get_str(NULL, 16, random_y));

    print_point(&random_point);

    bool is_random_point_on_curve = is_point_on_curve(curve, &random_point);
}


int main() {
    // bitcoin elliptic curve
    EllipticCurve curve; 
    init_elliptic_curve(&curve);

    // EC generator point
    Point g;
    init_point(&g, "G");
    set_point_hex(
        &g,
        "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798",
        "483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8"
    );

    set_elliptic_curve(
        &curve,
        "0",
        "7",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F",
        &g,
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141",
        "1"
    );

    print_point(&g);

    // ---------- ON CURVE CHECK ----------

    // random point on curve
    // random_point_on_curve_check(&curve);

    // G on curve
    bool is_g_on_curve = is_point_on_curve(&curve, &g);

    // addition tests
    Point added_g;
    init_point(&added_g, "G + G");

    // G + G
    ec_point_addition(&curve, &g, &g, &added_g);
    print_point(&added_g);

    is_point_on_curve(&curve, &added_g);

    // G * 2
    mpz_t two;
    mpz_init_set_ui(two, 2);

    Point g_2;
    init_point(&g_2, "G * 2");
    point_mult_scalar(&curve, &g, two, &g_2);
    print_point(&g_2);

    // G + G + G
    Point added_g_3;
    init_point(&added_g_3, "G + G + G");

    ec_point_addition(&curve, &added_g, &g, &added_g_3);
    print_point(&added_g_3);

    is_point_on_curve(&curve, &added_g_3);

    // ------------------------------------




    // secret key
    const char* secret_key = "Andrej is cool :P";
    size_t secret_key_length;

    unsigned char* secret_key_bytes = string_to_bytes(secret_key, &secret_key_length, ENDIAN_BIG);
    print_bytes(secret_key_bytes, secret_key_length);

    // convet to bignum & multiply with G to get public key
    mpz_t secret_key_bn;
    mpz_init(secret_key_bn);
    mpz_import(secret_key_bn, secret_key_length, 1, 1, 1, 0, secret_key_bytes);

    // print secret key
    gmp_printf("Secret Key: %Zd\n", secret_key_bn);

    Point public_key;
    init_point(&public_key, "Public Key");
    point_mult_scalar(&curve, &g, secret_key_bn, &public_key);

    print_point(&public_key);


    // SHA256 & RIPEMD160

    mpz_t input, output;
    mpz_init_set_str(input, "0", 10);
    mpz_init(output);

    mpz_sha256(input, output);

    // print output in hex
    gmp_printf("SHA256: 0x%Zx\n", output);



    return 0;
}
