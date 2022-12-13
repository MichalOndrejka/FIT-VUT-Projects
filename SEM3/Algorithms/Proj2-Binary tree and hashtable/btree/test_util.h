#ifndef IAL_BTREE_TEST_UTIL_H
#define IAL_BTREE_TEST_UTIL_H

#include "btree.h"
#include <stdio.h>

#define TEST(NAME, DESCRIPTION)                                                \
  void NAME() {                                                                \
    printf("[%s] %s\n", #NAME, DESCRIPTION);                                   \
    bst_node_t *test_tree;

#define ENDTEST                                                                \
  printf("\n");                                                                \
  bst_dispose(&test_tree);                                                     \
  }

typedef enum direction { left, right, none } direction_t;

void bst_print_subtree(bst_node_t *tree, char *prefix, direction_t from);
void bst_print_tree(bst_node_t *tree);
void bst_insert_many(bst_node_t **tree, const char keys[], const int values[],
                     int count);
#endif
