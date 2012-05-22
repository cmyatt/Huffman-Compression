#ifndef BUFFER_H
#define BUFFER_H

#include "codes.h"

typedef struct {
    uchar *buffer;
    int capacity, current_byte, current_bit;
} byte_buffer;

byte_buffer *new_buffer(int size);
void resize(byte_buffer *b);
void print_buffer(byte_buffer *b);
void save(byte_buffer *b, const char *filename);
void free_buffer(byte_buffer *b);

void append_bit_to_buffer(byte_buffer *b, uchar bit);
void append_byte_to_buffer(byte_buffer *b, uchar byte);
void append_to_buffer(byte_buffer *b, code *c);

uchar get_next_bit(byte_buffer *b);
uchar get_next_byte(byte_buffer *b);

#endif
