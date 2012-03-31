#include "run_length.h"
#include "../bits.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

byte_buffer *rl_compress(char *data, long size) {
	byte_buffer *buff = new_buffer((3 * size)/4);
	long i;
	int k;

	// 1. Determine the longest sequence of 1s or 0s.
	int longest_seq = 1, current_seq = 0;
	uchar current = is_bit_set(data[0], 0);
	for (i = 0; i < size; i++) {
		for (k = 0; k < 8; ++k) {
			if (is_bit_set(data[i], k) == current) {
				current_seq++;
			} else {
				if (current_seq > longest_seq)
					longest_seq = current_seq;
				current_seq = 1;
				current = (current == 0)? 1 : 0;
			}
		}
	}
	
	// 2. Determine number of bits required to represent that sequence.
	const ushort NUM_BITS = (const ushort)ceil(log((double)longest_seq)/log(2.0));
	
	// 3. First 16 bits used to store length of the run lengths in bits.
    uchar *byte_representation = (uchar *)(&NUM_BITS);
    for (i = 0; i < sizeof(NUM_BITS); ++i) {
        append_byte_to_buffer(buff, byte_representation[i]);
    }
	
	// 4. Start with 0s, write length, then write 1s length, etc.
	if (is_bit_set(data[0], 0)) {
		write_length(buff, 0, NUM_BITS);
    }

	current_seq = 0;
    current = is_bit_set(data[0], 0);
	for (i = 0; i < size; i++) {
        for (k = 0; k < 8; ++k) {
            if (is_bit_set(data[i], k) == current) {
                current_seq++;
            } else {
                write_length(buff, current_seq, NUM_BITS);
                current_seq = 1;
                current = is_bit_set(data[i], k);
            }
        }
	}
    
    if (current_seq > 0) {
        write_length(buff, current_seq, NUM_BITS);
    }
	
	return buff;
}


file *rl_decode(const char *data, long size) {
    int i;
    byte_buffer *buff = new_buffer(size);
    memcpy(buff->buffer, data, size);

    // 1. Get size of each run.
    uchar bytes[2];
    for (i = 0; i < 2; ++i) {
        bytes[i] = get_next_byte(buff);
    }
    const ushort NUM_BITS = *((ushort *)bytes);

    // 2. Decode data
    int zeros = 1;
    int remaining = 8 * size;
    int max_len = (4 * size)/3;

    byte_buffer *output = new_buffer(max_len);
    i = 0;

    while (remaining > NUM_BITS) {
        int bits = read_length(buff, NUM_BITS);
        for (i = 0; i < bits; ++i) {
            if (zeros) {
                append_bit_to_buffer(output, 0);
            } else {
                append_bit_to_buffer(output, 1);
            }
        }
        zeros = (zeros == 0)? 1 : 0;
        remaining -= NUM_BITS;
    }
    
    // 3. Convert output buffer to file
    file *f = (file *)malloc(sizeof(file));
    f->size = (long)output->current_byte;
    f->data = (char *)malloc(f->size);
    memcpy(f->data, output->buffer, f->size);
    
    // 4. Free resources
    free_buffer(output);
    free_buffer(buff);

    return f;
}


void write_length(byte_buffer *b, int length, ushort bits) {
	// Check endianess -- might need to reverse direction
	int i, k, num_bytes = (int)ceil((double)bits/8.0);
	uchar *bytes = (uchar *)(&length);
	
	for (i = 0; i < num_bytes; ++i) {
		for (k = 0; k < 8 && bits > 0; ++k) {
			append_bit_to_buffer(b, is_bit_set(bytes[i], k));
			bits--;
		}
	}
}


int read_length(byte_buffer *b, ushort bits) {
    int i;
    uchar bytes[4];
    memset(bytes, 0, 4);

    for (i = 0; i < bits; ++i) {
        if (get_next_bit(b)) {
            set_bit(bytes+(i/8), i%8);
        }
    }
    return *((int *)bytes);
}
