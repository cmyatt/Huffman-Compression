#include "file.h"


file *new_file(long size) {
    file *f = (file *) malloc(sizeof(file));
    f->size = size;
    if (size > 0) {
        f->data = (char *) malloc(f->size);
    }
    return f;
}


file *load(const char *filename) {
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        printf("Unable to open file %s\n", filename);
        return NULL;
    }

    // Get file size
    long size = 0;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    // Allocate memory to contain file
    char *data = (char *) malloc(size);
    memset(data, 0, size);
    
    // Read file data
	long s = fread(data, sizeof(char), size, f);
    if (s != size) {
        printf("%ld\n", s);
		printf("Not all of file %s read\n", filename);
        free(data);
        fclose(f);
        return NULL;
    }
    fclose(f);
    
    file *ret_val = (file *) malloc(sizeof(file));
    ret_val->data = data;
    ret_val->size = size;

    return ret_val;
}


void free_file(file *f) {
    free(f->data);
    free(f);
}
