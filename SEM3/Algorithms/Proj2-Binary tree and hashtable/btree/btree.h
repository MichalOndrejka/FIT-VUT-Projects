/*
 * Hlavičkový súbor pre binárny vyhľadávací strom.
 * Tento súbor neupravujte.
 */

#ifndef IAL_BTREE_H
#define IAL_BTREE_H

#include <stdbool.h>

// Uzol stromu
typedef struct bst_node {
  char key;               // kľúč
  int value;              // hodnota
  struct bst_node *left;  // ľavý potomok
  struct bst_node *right; // pravý potomok
} bst_node_t;

void bst_init(bst_node_t **tree);
void bst_insert(bst_node_t **tree, char key, int value);
bool bst_search(bst_node_t *tree, char key, int *value);
void bst_delete(bst_node_t **tree, char key);
void bst_dispose(bst_node_t **tree);

void bst_preorder(bst_node_t *tree);
void bst_inorder(bst_node_t *tree);
void bst_postorder(bst_node_t *tree);

void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree);

void bst_print_node(bst_node_t *node);

#endif
