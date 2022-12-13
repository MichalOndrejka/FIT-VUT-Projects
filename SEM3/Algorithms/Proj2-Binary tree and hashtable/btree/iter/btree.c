/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *temp = tree;
  while (temp != NULL) {
    if (temp->key == key) {
      *value = temp->value;
      return true;
    } else if (key < temp->key) {
      temp = temp->left;
    } else if (key > temp->key) {
      temp = temp->right;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (*tree == NULL) {
    *tree = malloc(sizeof(bst_node_t));
    if (*tree == NULL) return;
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
  } else {
    bst_node_t *temp = *tree;
    while (temp->key != key) {
      if (key < temp->key) {
        if (temp->left == NULL) {
          bst_node_t *new = malloc(sizeof(bst_node_t));
          if (new == NULL) return;
          new->key = key;
          new->value = value;
          new->left = NULL;
          new->right = NULL;
          temp->left = new;
          return;
        }
        temp = temp->left;
      } 
      else if (key > temp->key) {
        if (temp->right == NULL) {
          bst_node_t *new = malloc(sizeof(bst_node_t));
          if (new == NULL) return;
          new->key = key;
          new->value = value;
          new->left = NULL;
          new->right = NULL;
          temp->right = new;
          return;
        }
        temp = temp->right;
      }
    }
    (*tree)->value = value;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *temp = *tree;
  bst_node_t *node_before = *tree;
  while (temp->right != NULL) {
    node_before = temp;
    temp = temp->right;
  }
  target->key = temp->key;
  target->value = temp->value;
  if (node_before != NULL) node_before->right = NULL;
  node_before->right = temp->left;
  free(temp);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree == NULL) return;

  bst_node_t *temp = *tree;
  bst_node_t *node_before = temp;
  bool left;

  while (temp != NULL) {
    if (temp->key == key) break;
    else if (key < temp->key) {
      node_before = temp;
      temp = temp->left;
      left = true;
    }
    else if (key > temp->key) {
      node_before = temp;
      temp = temp->right;
      left = false;
    }
  }

  if (temp == NULL) return;

  if (temp->left != NULL && temp->right != NULL) {
    bst_replace_by_rightmost(temp, &(temp->left));
  }
  else {
    if (temp->right != NULL) {
      if (left == true) node_before->left = temp->right;
      else node_before->right = temp->right;
    } else {
      if (left == true) node_before->left = temp->left;
      else node_before->right = temp->left;
    }
    free(temp);
  }
}
/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if (*tree == NULL) return;
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);
  bst_node_t *temp = *tree;
  while (stack_bst_empty(stack) == false || temp != NULL) {
    if (temp == NULL) temp = stack_bst_pop(stack);
    if (temp->right != NULL) stack_bst_push(stack, temp->right);
    bst_node_t *delete = temp;
    temp = temp->left;
    free(delete);
  }
  *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *temp = tree;
  while (temp != NULL) {
    bst_print_node(temp);
    stack_bst_push(to_visit, temp);
    temp = temp->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack);
  while (stack_bst_empty(stack) != true) {
    tree = stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack);
  }

  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *temp = tree;
  while (temp != NULL) {
    stack_bst_push(to_visit, temp);
    temp = temp->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);
  bst_leftmost_inorder(tree, stack);
  while(stack_bst_empty(stack) != true) {
    tree = stack_bst_pop(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, stack);
  }

  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  bst_node_t *temp = tree;
  while(temp != NULL) {
    stack_bst_push(to_visit, temp);
    stack_bool_push(first_visit, true);
    temp = temp->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);

  stack_bool_t *bool_stack = malloc(sizeof(stack_bool_t));
  if (bool_stack == NULL) return;
  stack_bool_init(bool_stack);

  bst_node_t *temp = tree;
  bool temp_bool;
  bst_leftmost_postorder(temp, stack, bool_stack);

  while (stack_bst_empty(stack) != true) {
    temp = stack_bst_pop(stack);
    temp_bool = stack_bool_pop(bool_stack);
    if (temp_bool == true) {
      stack_bst_push(stack, temp);
      stack_bool_push(bool_stack, false);
      bst_leftmost_postorder(temp->right, stack, bool_stack);
    } else bst_print_node(temp);
  }
  
  free(stack);
  free(bool_stack);
}
