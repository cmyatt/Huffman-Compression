#ifndef CODES_H
#define CODES_H

#include "defs.h"

typedef struct {
    uchar *memory;
    int num_bits, num_bytes, size;
} code;

code *new_code();
code *duplicate_code(code *c);
void append_bit(code *c, uchar b);
void remove_bit(code *c);
void print_code(code *c);
void free_code(code *c);

#endif
