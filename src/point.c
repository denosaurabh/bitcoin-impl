#include "point.h"

#include <string.h>
#include <assert.h>
#include <gmp.h>
#include "ecdsa.h"



void init_point(Point *point, const char *name) {
    mpz_init(point->x);
    mpz_init(point->y);
    strncpy(point->name, name, MAX_POINT_NAME_LENGTH-1);
    point->name[MAX_POINT_NAME_LENGTH-1] = '\0';
}

void set_point_hex(
    Point *point,
    const char *x,
    const char *y
) {
    mpz_set_str(point->x, x, 16);
    mpz_set_str(point->y, y, 16);
}

void clear_point(Point *point) {
    mpz_clear(point->x);
    mpz_clear(point->y);
    memset(point->name, 0, MAX_POINT_NAME_LENGTH);
}

void copy_point(
    Point *src,
    Point *dst
) {
    mpz_set(dst->x, src->x);
    mpz_set(dst->y, src->y);
}


void print_point(Point *point) {
    // print hex
    // gmp_printf("Point (%s): (0x%Zx, 0x%Zx)\n", point->name, point->x, point->y);

    // print decimal
    gmp_printf("Point (%s): (%Zd, %Zd)\n", point->name, point->x, point->y);
}

