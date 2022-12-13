/*
 * Hlavičkový súbor pre binárny vyhľadávací strom.
 * Tento súbor neupravujte.
 */


#pragma once
#include <stdbool.h>
#include <stdlib.h>
typedef enum{
    T_VOID, T_INT, T_FLOAT, T_STRING
} type;

typedef struct {
    type type;
    bool param_is_q;
} token;

typedef struct NODE_LOCAL
{
    char *key;
    struct NODE_LOCAL *left;
    struct NODE_LOCAL *right;
    token *token;
} NODE_LOCAL;

typedef struct NODE{
    char *key;
    struct NODE *left;
    struct NODE *right;
    int param_count;
    type return_type;
    bool return_type_is_q;
    type params[32];
    char *params_names[32];

    NODE_LOCAL *local;

    //token *tokens;
} NODE;

void bst_init(NODE **tree);
void bst_insert(NODE **tree, char *key); //key je nazov aktualnej funkcie
void bst_insert_local(NODE_LOCAL **tree, char *key, token *token); //key je nazov vkladaneho parametru albeo premennej



NODE *bst_search(NODE *tree, char *key);
token *bst_search_local(NODE_LOCAL *tree, char *key);


void bst_delete(NODE **tree, char *key);
void bst_dispose(NODE **tree);
void bst_dispose_local(NODE_LOCAL **tree);


void bst_replace_by_rightmost(NODE *target, NODE **tree);

void bst_print_node(NODE *node);

