#ifndef CODES_H
#define CODES_H

#include "defs.h"


code *new_code() {
    code *c = (code *) malloc(sizeof(code));
    c->memory = (uchar *) malloc(sizeof(uchar));
    c->num_bits = 0;
    c->num_bytes = 1;
    c->size = 1;
    return c;
}


code *duplicate_code(code *c) {
    code *copy = (code *) malloc(sizeof(code));
    copy->memory = (uchar *) malloc(sizeof(uchar) * c->num_bytes);
    memcpy(copy->memory, c->memory, c->num_bytes);
    copy->num_bits = c->num_bits;
    copy->num_bytes = c->num_bytes;
    copy->size = c->num_bytes;
    return copy;
}


void append_bit(code *c, uchar b) {
    uchar pos = (uchar) (c->num_bits % 8);
    c->num_bits++;
    
    if (b) {
        set_bit(c->memory + (c->num_bytes - 1), pos);
    } else {
        clear_bit(c->memory + (c->num_bytes - 1), pos);
    }

    // Allocate more memory if needed
    if (c->num_bits % 8 == 0) {
        if (c->num_bytes == c->size) {
            c->size++;
            uchar temp[c->num_bytes];
            int i;

            for (i = 0; i < c->num_bytes; ++i) {
                temp[i] = c->memory[i];
            }

            free(c->memory);
            c->memory = (uchar *) malloc(sizeof(uchar) * c->size);

            for (i = 0; i < c->num_bytes; ++i) {
                c->memory[i] = temp[i];
            }
            c->memory[c->num_bytes] = 0;
        }
        c->num_bytes++;
    }
}


void remove_bit(code *c) {
    if (c->num_bits == 0) return;
    if (c->num_bits%8 == 0 && c->num_bytes > 1) {
        c->num_bytes--;
    }
    c->num_bits--;
    clear_bit(c->memory + (c->num_bytes - 1), c->num_bits%8);
}


void print_code(code *c) {
    int i, byte, pos;
    for (i = 0; i < c->num_bits; ++i) {
        byte = i / 8;
        pos = i % 8;
        printf("%d", is_bit_set(c->memory[byte], pos));
    }
}


void free_code(code *c) {
    free(c->memory);
    free(c);
}

#endif
