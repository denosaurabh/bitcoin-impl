#include "str.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


unsigned char* string_to_bytes(const char* str, size_t* byte_length, Endianness endian) {
    if (str == NULL || byte_length == NULL) {
        return NULL;
    }

    size_t str_len = strlen(str);
    unsigned char* bytes = (unsigned char*)malloc(str_len);
    
    if (bytes == NULL) {
        return NULL;  // Memory allocation failed
    }

    if (endian == ENDIAN_BIG) {
        memcpy(bytes, str, str_len);
    } else {
        // Little-endian: reverse the byte order
        for (size_t i = 0; i < str_len; i++) {
            bytes[i] = str[str_len - 1 - i];
        }
    }

    *byte_length = str_len;
    return bytes;
}

void print_bytes(const unsigned char* bytes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

