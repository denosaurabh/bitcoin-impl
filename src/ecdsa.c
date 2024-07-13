#include "ecdsa.h"

#include <gmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "point.h"


void init_elliptic_curve(EllipticCurve *curve) {
    mpz_init(curve->a);
    mpz_init(curve->b);
    mpz_init(curve->p);

    mpz_init(curve->n);
    mpz_init(curve->h);
}

void set_elliptic_curve(
    EllipticCurve *curve,
    const char *a,
    const char *b,
    const char *p,
    Point *g,
    const char *n,
    const char *h
) {
    mpz_set_str(curve->a, a, 16);
    mpz_set_str(curve->b, b, 16);
    mpz_set_str(curve->p, p, 16);

    curve->g = *g;

    mpz_set_str(curve->n, n, 16);
    mpz_set_str(curve->h, h, 16);
}

bool is_point_on_curve(EllipticCurve *curve, Point *point) {
    // y^2
    mpz_t bn2;
    mpz_init_set_ui(bn2, 2); // 2 in bignum form

    mpz_t y2;
    mpz_init(y2);
    mpz_powm(y2, point->y, bn2, curve->p);

    // x^3
    mpz_t bn3;
    mpz_init_set_ui(bn3, 3);

    mpz_t x3;
    mpz_init(x3);
    mpz_powm(x3, point->x, bn3, curve->p);

    // (y^2 - x^3 - 7) % p
    mpz_t left_side;
    mpz_init(left_side);
    mpz_sub(left_side, y2, x3);
    mpz_sub(left_side, left_side, curve->b);
    mpz_mod(left_side, left_side, curve->p);

    bool is_on_curve = mpz_cmp_ui(left_side, 0) == 0;

    // clean up
    mpz_clear(bn2);
    mpz_clear(y2);
    mpz_clear(bn3);
    mpz_clear(x3);
    mpz_clear(left_side);
    
    printf("is (%s) on curve: %s\n", point->name, is_on_curve ? "true" : "false");

    return is_on_curve;
}

bool compare_points(Point *p1, Point *p2) {
    return mpz_cmp(p1->x, p2->x) == 0 && mpz_cmp(p1->y, p2->y) == 0;
}

bool compare_points_x(Point *p, Point *q) {
    return mpz_cmp(p->x, q->x) == 0;
}

bool compare_points_y(Point *p, Point *q) {
    return mpz_cmp(p->y, q->y) == 0;
}


// elliptic curve point addition
void ec_point_addition(EllipticCurve *curve, Point *p, Point *q, Point *r) {
    if (compare_points(p, &CURVE_INFINITY)) {
        copy_point(q, r);
        return;
    }

    if (compare_points(q, &CURVE_INFINITY)) {
        copy_point(p, r);
        return;
    }

    if (compare_points_x(p, q) && !compare_points_y(p, q)) {
        copy_point(&CURVE_INFINITY, r);
        return;
    }

    mpz_t m, x3, y3, temp;
    mpz_init(m);
    mpz_init(x3);
    mpz_init(y3);
    mpz_init(temp);

    if (compare_points(p, q)) {
        // Point doubling
        mpz_t left, right;
        mpz_init(left);
        mpz_init(right);

        // 3 * x^2 + a
        mpz_mul(temp, p->x, p->x);
        mpz_mod(temp, temp, curve->p);
        mpz_mul_ui(left, temp, 3);
        mpz_add(left, left, curve->a);
        mpz_mod(left, left, curve->p);

        // 2 * y
        mpz_mul_ui(right, p->y, 2);
        mpz_mod(right, right, curve->p);

        // m = (3 * x^2 + a) / (2 * y)
        mpz_invert(right, right, curve->p);
        mpz_mul(m, left, right);
        mpz_mod(m, m, curve->p);

        mpz_clear(left);
        mpz_clear(right);
    } else {
        // Point addition
        mpz_sub(temp, q->y, p->y);
        mpz_mod(temp, temp, curve->p);
        
        mpz_t denominator;
        mpz_init(denominator);
        mpz_sub(denominator, q->x, p->x);
        mpz_mod(denominator, denominator, curve->p);
        
        mpz_invert(denominator, denominator, curve->p);
        mpz_mul(m, temp, denominator);
        mpz_mod(m, m, curve->p);

        mpz_clear(denominator);
    }

    // x3 = m^2 - p.x - q.x
    mpz_mul(x3, m, m);
    mpz_sub(x3, x3, p->x);
    mpz_sub(x3, x3, q->x);
    mpz_mod(x3, x3, curve->p);

    // y3 = m * (p.x - x3) - p.y
    mpz_sub(temp, p->x, x3);
    mpz_mul(y3, m, temp);
    mpz_sub(y3, y3, p->y);
    mpz_mod(y3, y3, curve->p);

    // Set result point
    mpz_set(r->x, x3);
    mpz_set(r->y, y3);

    // Clean up
    mpz_clear(m);
    mpz_clear(x3);
    mpz_clear(y3);
    mpz_clear(temp);
}


void point_mult_scalar(
    EllipticCurve *curve,
    Point *p,
    mpz_t k,
    Point *res
) {
    mpz_t k_copy;
    mpz_init_set(k_copy, k);

    Point result;
    init_point(&result, "result");
    copy_point(&CURVE_INFINITY, &result);  // Start with the point at infinity

    Point append;
    init_point(&append, "append");
    copy_point(p, &append);
    
    while (mpz_cmp_ui(k_copy, 0) > 0) {
        if (mpz_tstbit(k_copy, 0)) {
            ec_point_addition(curve, &result, &append, &result);
        }

        ec_point_addition(curve, &append, &append, &append);
        
        // Use a temporary variable for the division
        mpz_t temp;
        mpz_init(temp);
        mpz_tdiv_q_2exp(temp, k_copy, 1);
        mpz_set(k_copy, temp);
        mpz_clear(temp);

        // Add a safeguard
        if (mpz_cmp_ui(k_copy, 0) == 0) {
            break;
        }
    }

    copy_point(&result, res);

    // Clean up
    mpz_clear(k_copy);
    clear_point(&result);
    clear_point(&append);
}

