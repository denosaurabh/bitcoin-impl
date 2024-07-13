#ifndef POINT_H
#define POINT_H

#include <gmp.h>


#define MAX_POINT_NAME_LENGTH 100

typedef struct {
    char name[MAX_POINT_NAME_LENGTH];
    mpz_t x;
    mpz_t y;
} Point;


// initializers
void init_point(Point *point, const char *name);
void set_point_hex(Point *point, const char *x, const char *y);
void copy_point(Point *src, Point *dst);
void clear_point(Point *point);

// utils
void print_point(Point *point);


#endif
