/*
 * Hlavičkový súbor pre pomocné zásobníky.
 * Tento súbor neupravujte.
 */
#ifndef IAL_BTREE_ITER_STACK_H
#define IAL_BTREE_ITER_STACK_H

#include "../btree.h"

// Maximálna veľkosť zásobníku
#define MAXSTACK 30

/*
 * Makro generujúce deklarácie pre zásobník typu T s názvovým infixom TNAME.
 * Pre TNAME="bst" pracujúce s typom T="bst_node_t*":
 *   Dátový typ stack_bst_t
 *   Funkcie void stack_bst_init(stack_bst_t *stack)
 *           void stack_bst_push(stack_bst_t *stack, bst_node_t *item)
 *           bst_node_t *stack_bst_pop(stack_bst_t *stack)
 *           bst_node_t *stack_bst_top(stack_bst_t *stack)
 *           bool stack_bst_empty(stack_bst_t *stack)
 * A ekvivalent pre TNAME="bool", T="bool".
 */
#define STACKDEC(T, TNAME)                                                     \
  typedef struct {                                                             \
    T items[MAXSTACK];                                                         \
    int top;                                                                   \
  } stack_##TNAME##_t;                                                         \
                                                                               \
  void stack_##TNAME##_init(stack_##TNAME##_t *stack);                         \
  void stack_##TNAME##_push(stack_##TNAME##_t *stack, T item);                 \
  T stack_##TNAME##_pop(stack_##TNAME##_t *stack);                             \
  T stack_##TNAME##_top(stack_##TNAME##_t *stack);                             \
  bool stack_##TNAME##_empty(stack_##TNAME##_t *stack);

STACKDEC(bst_node_t *, bst)
STACKDEC(bool, bool)

#endif
