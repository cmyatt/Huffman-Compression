#ifndef RUN_LENGTH_H
#define RUN_LENGTH_H

#include "../buffer.h"
#include "../file.h"
#include "../defs.h"

void write_length(byte_buffer *b, int length, ushort bits);
int read_length(byte_buffer *b, ushort bits);

byte_buffer *rl_compress(char *data, long size);
file *rl_decode(const char *data, long size);

#endif
