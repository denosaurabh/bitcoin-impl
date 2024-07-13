#ifndef ECDSA_H
#define ECDSA_H

#include <stdbool.h>
#include <gmp.h>

#include "point.h"



/// Elliptic Curve
/// General Form: y^2 = x^3 + ax + b
typedef struct {
    // coefficients a & b
    mpz_t a;
    mpz_t b;

    // modulo p (prime number)
    mpz_t p;

    // generator point
    Point g;

    // order of the generator point (n = #G)
    mpz_t n;
    // cofactor of the generator point (h = #E(Fp) / n)
    mpz_t h;
} EllipticCurve;


#define CURVE_INFINITY (Point) { "infinity", 0, 0 }


void init_elliptic_curve(EllipticCurve *curve);
void set_elliptic_curve(
    EllipticCurve *curve,
    const char *a,
    const char *b,
    const char *p,
    Point *g,
    const char *n,
    const char *h
);

bool is_point_on_curve(EllipticCurve *curve, Point *point);


// maths
void ec_point_addition(EllipticCurve *curve, Point *p, Point *q, Point *r);
void point_mult_scalar(EllipticCurve *curve, Point *point, mpz_t scalar, Point *result);



bool compare_points(Point *p1, Point *p2);
bool compare_points_x(Point *p1, Point *p2);
bool compare_points_y(Point *p1, Point *p2);



#endif
