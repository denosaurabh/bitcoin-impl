#ifndef STR_H
#define STR_H


#include <string.h>

// STRING UTILS
//
typedef enum {
    ENDIAN_BIG,
    ENDIAN_LITTLE
} Endianness;


unsigned char* string_to_bytes(const char* str, size_t* byte_length, Endianness endian); 
void print_bytes(const unsigned char* bytes, size_t length);



#endif
