#include "codes.h"
#include "buffer.h"


byte_buffer *new_buffer(int size) {
    byte_buffer *b = (byte_buffer *) malloc(sizeof(byte_buffer));
    b->buffer = (uchar *) malloc(sizeof(uchar) * size);
    memset(b->buffer, 0, size*sizeof(uchar));
    b->capacity = size;
    b->current_byte = b->current_bit = 0;
    return b;
}


void resize(byte_buffer *b) {
    if (b->current_byte >= b->capacity) {
        int i, num_elems=b->capacity;
        uchar temp[num_elems];
        
        for (i = 0; i < num_elems; ++i) {
            temp[i] = b->buffer[i];
        }

        free(b->buffer);
        b->capacity *= MEM_COEFF;
        b->buffer = (uchar *) malloc(sizeof(uchar) * b->capacity);
        memset(b->buffer, 0, b->capacity);

        for (i = 0; i < num_elems; ++i) {
            b->buffer[i] = temp[i];
        }
    }
}


void append_bit_to_buffer(byte_buffer *b, uchar bit) {
    if (b->current_bit > 7) {
        b->current_byte++;
        resize(b);
        b->current_bit = 0;
    }
    
    if (bit) {
        set_bit(b->buffer + b->current_byte, b->current_bit);
    }
    
    b->current_bit++;
}


void append_byte_to_buffer(byte_buffer *b, uchar byte) {
    int i;
    for (i = 0; i < 8; ++i) {
        append_bit_to_buffer(b, is_bit_set(byte, i));
    }
}


void append_to_buffer(byte_buffer *b, code *c) {
    int i, byte, pos;
    for (i = 0; i < c->num_bits; ++i) {
        byte = i / 8;
        pos = i % 8;
        append_bit_to_buffer(b, is_bit_set(c->memory[byte], pos));
    }
}


uchar get_next_bit(byte_buffer *b) {
    if (b->current_byte > b->capacity) return -1;

    uchar bit = is_bit_set(b->buffer[b->current_byte], b->current_bit);
    b->current_bit++;

    if (b->current_bit > 7) {
        b->current_byte++;
        b->current_bit = 0;
    }
    return bit;
}


uchar get_next_byte(byte_buffer *b) {
    if (b->current_byte > b->capacity) return -1;
    
    int i;
    uchar byte = 0;

    for (i = 0; i < 8; ++i) {
        if (get_next_bit(b) == 1) {
            set_bit(&byte, i);
        }
    }
    return byte;
}


void print_buffer(byte_buffer *b) {
    int i, k;
    for (i = 0; i <= b->current_byte; ++i) {
        if (i == b->current_byte) {
            for (k = 0; k < b->current_bit; ++k) {
                printf("%d", is_bit_set(b->buffer[i], k));
            }
        } else {
            for (k = 0; k < 8; ++k) {
                printf("%d", is_bit_set(b->buffer[i], k));
            }
            if ((i+1)%4 == 0) {
                printf("\n");
            } else {
                printf("  ");
            }
        }
    }
    printf("\n");
}


void save(byte_buffer *b, const char *filename) {
    int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    int num_bytes = b->current_byte;
    num_bytes += (b->current_bit == 0)? 0 : 1;

    if (fd == -1) {
        printf("Error opening file '%s'\n", filename);
        return;
    }

    if (write(fd, b->buffer, num_bytes) == -1) {
        printf("Error writing buffer to file '%s'\n", filename);
    }
    
    if (close(fd) == -1) {
        printf("Error closing file '%s'\n", filename);
    }
}


void free_buffer(byte_buffer *b) {
    free(b->buffer);
    free(b);
}
