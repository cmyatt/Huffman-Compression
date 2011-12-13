#ifndef DEFS_H
#define DEFS_H

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#include "../reuse/bits.h"

#define MEM_COEFF 2
#define NUM_FREQS 256
#define NO_CHAR   300


typedef unsigned short ushort;


typedef struct {
    char *data;
    long size;
} file;


typedef struct {
    char *name;
    long start, end;
    long tick_count;
    float secs;
} profile_t;


typedef struct {
    profile_t *list;
    int capacity, size;
} profiler;


typedef struct {
    uchar *memory;
    int num_bits, num_bytes, size;
} code;


typedef struct {
    uchar *buffer;
    int capacity, current_byte, current_bit;
} byte_buffer;


typedef struct node_t {
    ushort value;
    int freq;
    struct node_t *left, *right;
} node;


/* file.h
*/
file *new_file(long size);
file *load(const char *filename);
void free_file(file *f);


/* profiler.h
*/
void init_profile_t(profile_t *p, char *name);
void update_profile_t(profile_t *p);
void print_profile_t(profile_t *p);

void init_profiler(profiler *p, int num_profiles);
void start_profile(profiler *p, char *name);
void stop_profile(profiler *p, char *name);
void print_profile(profiler *p, char *name);
void print_all(profiler *p);
void free_profiler(profiler *p);


/* codes.h
*/
code *new_code();
code *duplicate_code(code *c);
void append_bit(code *c, uchar b);
void remove_bit(code *c);
void print_code(code *c);
void free_code(code *c);


/* buffer.h
*/
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


/* huffman.h
*/
node *new_node(ushort val, int freq, node *l, node *r);
int comp_nodes(const void *elem1, const void *elem2);
void sort_nodes(node **n, int length);
void free_node(node *n);

void output_tree(node *n, byte_buffer *b);
void make_codes(node *n, code **list, code *current);
char *join_str(char *a, char *b);
int construct_tree(node *n, byte_buffer *b);

byte_buffer *compress(char *data, long size);
file *decode(const char *data, long size);

#endif
