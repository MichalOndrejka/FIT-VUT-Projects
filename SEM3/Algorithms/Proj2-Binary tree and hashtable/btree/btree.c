#include "btree.h"
#include <stdio.h>

/*
 * Pomocná funkcia ktorá vypíše uzol stromu.
 */
void bst_print_node(bst_node_t *node) {
  printf("[%c,%d]", node->key, node->value);
}
