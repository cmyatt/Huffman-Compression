#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "defs.h"
#include "node.h"
#include "file.h"
#include "codes.h"
#include "buffer.h"

void output_tree(node *n, byte_buffer *b);
void make_codes(node *n, code **list, code *current);
char *join_str(char *a, char *b);
int construct_tree(node *n, byte_buffer *b);

byte_buffer *compress(char *data, long size);
file *decode(const char *data, long size);

#endif
