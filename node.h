#ifndef NODE_H
#define NODE_H

#include "defs.h"

typedef struct node_t {
    ushort value;
    int freq;
    struct node_t *left, *right;
} node;

node *new_node(ushort val, int freq, node *l, node *r);
int comp_nodes(const void *elem1, const void *elem2);
void sort_nodes(node **n, int length);
void free_node(node *n);

#endif
