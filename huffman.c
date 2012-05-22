#include "file.h"
#include "huffman.h"
#include "profiler.h"


void output_tree(node *n, byte_buffer *b) {
    if (n->value != NO_CHAR) {
        int i;
        append_bit_to_buffer(b, 1);
        for (i = 0; i < 8; ++i) {
            append_bit_to_buffer(b, is_bit_set(n->value, i));
        }
    } else {
        append_bit_to_buffer(b, 0);
        output_tree(n->left, b);
        
        append_bit_to_buffer(b, 0);
        output_tree(n->right, b);
    }
}


void make_codes(node *n, code **list, code *current) {
    if (n->value != NO_CHAR) {
        list[n->value] = duplicate_code(current);
    } else {
        append_bit(current, 1);
        make_codes(n->left, list, current);
        remove_bit(current);
        
        append_bit(current, 0);
        make_codes(n->right, list, current);
        remove_bit(current);
    }
}


// Returns a new string made up of the merged strings a and b
char *join_str(char *a, char *b) {
    char *new_str = (char *) malloc(strlen(a) + strlen(b) + 1);
    new_str[0] = '\0';
    strcat(new_str, a);
    strcat(new_str, b);
    return new_str;
}


void count_nodes(node *n, int *count) {
    if (n->left) {
        *count += 1;
        count_nodes(n->left, count);
    }
    if (n->right) {
        *count += 1;
        count_nodes(n->right, count);
    }
}


byte_buffer *compress(char *data, long size) {
    byte_buffer *buff = new_buffer(size / 2);
    int i, frequencies[NUM_FREQS], num_nodes=0, index=0;
    node *head = NULL;

    profiler prof;
    init_profiler(&prof, 6);
    start_profile(&prof, "node creation");
    
    // Get frequency of each character
    memset(frequencies, 0, NUM_FREQS*sizeof(int));
    for (i = 0; i < size; ++i) {
        frequencies[(uchar)data[i]] += 1;
    }

    // Create node for each character
    for (i = 0; i < NUM_FREQS; ++i) {
        if (frequencies[i] != 0) {
            num_nodes++;
        }
    }
    node *node_list[num_nodes];
    
    for (i = 0; i < NUM_FREQS; ++i) {
        if (frequencies[i] != 0) {
            node_list[index] = new_node(i, frequencies[i], NULL, NULL);
            index++;
        }
    }
    sort_nodes(node_list, num_nodes);

    stop_profile(&prof, "node creation");
    start_profile(&prof, "tree creation");

    // Create huffman tree
    while (num_nodes > 1) {
        node *left=node_list[num_nodes-1], *right=node_list[num_nodes-2];
        int new_freq = left->freq + right->freq;

        node *temp = new_node(NO_CHAR, new_freq, left, right);
        num_nodes--;
        node_list[num_nodes-1] = temp;
        sort_nodes(node_list, num_nodes);
    }
    head = node_list[0];

    stop_profile(&prof, "tree creation");
    start_profile(&prof, "tree output");

    // Output representation of tree to buffer
    output_tree(head, buff);

    // Output the number of characters compressed
    // so decoder knows when to stop reading
    int i_size = (int)size;
    int *num_chars = &(i_size);
    uchar *byte_representation = (uchar *)num_chars;
    for (i = 0; i < 4; ++i) {
        append_byte_to_buffer(buff, byte_representation[i]);
    }

    stop_profile(&prof, "tree output");
    start_profile(&prof, "code creation");

    // Map characters to codes
    code *list[NUM_FREQS], *current=new_code();
    memset(list, 0, NUM_FREQS*sizeof(code *));
    make_codes(head, list, current);

    stop_profile(&prof, "code creation");
    start_profile(&prof, "data translation");

    // Translate data to output
    for (i = 0; i < size; ++i) {
        append_to_buffer(buff, list[(uchar)data[i]]);
    }

    stop_profile(&prof, "data translation");
    print_all(&prof);

    // Free memory
    free_node(head);
    free_code(current);
    for (i = 0; i < NUM_FREQS; ++i) {
        if (list[i] != 0) {
            free_code(list[i]);
        }
    }

    return buff;
}


int construct_tree(node *n, byte_buffer *b) {
    if (n->left != NULL && n->right != NULL) return 1;
    
    if (get_next_bit(b) == 0) {
       if (n->left == NULL) {
            n->left = new_node(NO_CHAR, 0, NULL, NULL);
            construct_tree(n->left, b);
            construct_tree(n, b);
        } else {
            n->right = new_node(NO_CHAR, 0, NULL, NULL);
            construct_tree(n->right, b);
            return 1;
        }
    } else {
        n->value = get_next_byte(b);
        return 1;
    }
}


file *decode(const char *data, long size) {
    int i;
    uchar bit = 0;
    byte_buffer *buff = new_buffer(size);
    node *head=new_node(NO_CHAR, 0, NULL, NULL), *current=head;

    profiler prof;
    init_profiler(&prof, 3);
    start_profile(&prof, "tree construction");
    
    // Copy encoded data into buffer and create huffman tree
    memcpy(buff->buffer, data, size);
    construct_tree(head, buff);

    stop_profile(&prof, "tree construction");

    // Get number of characters to decode
    uchar byte_representation[4];
    for (i = 0; i < 4; ++i) {
        byte_representation[i] = get_next_byte(buff);
    }
    int *num_chars = (int *)byte_representation;

    // Decode data
    int max_len=size*2;
    
    file *f = (file *) malloc(sizeof(file));
    f->data = (char *) malloc(max_len);
    memset(f->data, 0, max_len);
    f->size = 0;
    
    start_profile(&prof, "decoding");

    while (bit != 255) {
        if (f->size == (long)*num_chars) {
            break;
        }
        bit = get_next_bit(buff);
        current = (bit)? current->left : current->right;

        if (current->value != NO_CHAR) {
            f->size++;

            // Reallocate memory
            if (f->size >= max_len) {
                char str[f->size-1];
                memcpy(str, f->data, f->size-1);

                free(f->data);
                max_len *= 2;
                f->data = (char *) malloc(max_len);
                memset(f->data, 0, max_len);

                memcpy(f->data, str, f->size-1);
            }

            f->data[f->size-1] = current->value;
            current = head;
        }
    }

    stop_profile(&prof, "decoding");
    print_all(&prof);
    
    free_buffer(buff);
    free_node(head);

    return f;
}
