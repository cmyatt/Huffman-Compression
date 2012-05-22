#include "defs.h"
#include "file.h"
#include "buffer.h"
#include "huffman.h"

void usage() {
    printf("[usage] ./huffman_test <input_name>\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return 0;
    }

    char *filename;
    file *raw, *comp, *output;
    byte_buffer *b;
    long raw_size;

    filename = (char *) malloc(strlen(argv[1]) + 1);
    memset(filename, 0, strlen(argv[1]) + 1);
    strcat(filename, argv[1]);

    // Compress file
    raw = load(filename);
    b = compress(raw->data, raw->size);
    raw_size = raw->size;
    free_file(raw);

    // Save file in directory './data/output'
    free(filename);
    filename = (char *) malloc(strlen(argv[1]) + 4);
    memset(filename, 0, strlen(argv[1]) + 4);
    strcat(filename, argv[1]);
    strcat(filename, ".out");
    save(b, filename);

    // Decode output
    comp = load(filename);
    free(filename);
    output = decode(comp->data, comp->size);

    // Print stats
    printf("Original size: %ld bytes\nCompressed size: %ld bytes\n", raw_size, comp->size);
    printf("Compression factor: %f\n", (float)raw_size/(float)comp->size);
    printf("Compressed correctly: %ld\n", output->size-raw_size);
    
    // Free memory
    free_file(comp);
    free_file(output);
    free_buffer(b);

    return 0;
}
