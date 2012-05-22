#ifndef FILE_H
#define FILE_H

#include "defs.h"

typedef struct {
    char *data;
    long size;
} file;

file *new_file(long size);
file *load(const char *filename);
void free_file(file *f);

#endif
