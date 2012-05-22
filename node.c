#include "defs.h"
#include "node.h"


node *new_node(ushort val, int freq, node *l, node *r) {
    node *n = (node *) malloc(sizeof(node));
    n->value = val;
    n->freq = freq;
    n->left = l;
    n->right = r;
    return n;
}


int comp_nodes(const void *elem1, const void *elem2) {
    node *a = *((node **)elem1);
    node *b = *((node **)elem2);
    return b->freq - a->freq;
}


void sort_nodes(node **n, int length) {
    qsort(n, length, sizeof(node *), comp_nodes);
}


void free_node(node *n) {
    if (n->left != NULL && n->right != NULL) {
        free_node(n->left);
        free_node(n->right);
    }
    free(n);
}
