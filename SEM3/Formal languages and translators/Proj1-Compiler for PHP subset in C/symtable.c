/* ************************ symtable.c *********************************** */
/* ********** Implemetácia prekladaču imperatívneho jazyka IFJ22 ********* */
/*  Predmet: Formálne jazyky a prekladače (IFJ) - FIT VUT v Brne           */
/*  Team: xondre15, xtruch01, xbrnak01, xhrach06                           */
/*  Vytvoril: xhrach06, xbrnak01                                           */
/* *********************************************************************** */
#pragma once
#include "symtable.h"

///@bst_init = initialize the tree
void bst_init(NODE **tree) {
  *tree = NULL;
} 

NODE *bst_search(NODE *tree, char *key) {
	
  if (!tree)
	{
		return NULL;
	}

	if (strcmp(tree->key, key) == 0)
	{
		return tree;
	}
	if (strcmp(tree->key, key) < 0)
	{
		return bst_search(tree->left, key);
	}
	return bst_search(tree->right, key);
}

token *bst_search_local(NODE_LOCAL *tree, char *key){

	 if (!tree)
	{
		return NULL;
	}

	if (strcmp(tree->key, key) == 0)
	{
		return tree->token;
	}
	if (strcmp(tree->key, key) < 0)
	{
		return bst_search_local(tree->left, key);
	}
	return bst_search_local(tree->right, key);

}

void bst_insert(NODE **tree, char *key) {
  if (!*tree){
    (*tree) = malloc(sizeof(NODE));
    if (!(tree)){
	  exit(99);
      return;
    }
	
	(*tree)->key = malloc(sizeof(strlen(key)));
	if ((*tree)->key == NULL) {
		exit(99);
	}
	strcpy((*tree) -> key, key);
    //printf("%s\n", (*tree)->key);

	(*tree)->local = NULL;
	(*tree)->return_type = T_INT;
	(*tree) -> left = (*tree) -> right = NULL;

  }
	if (strcmp((*tree)->key, key) == 0) 
	{
		//error();
		return;
	}

	if (strcmp((*tree)->key, key) < 0) 
	{
		bst_insert(&((*tree) -> left), key);
		return;
	}
	bst_insert(&((*tree)->right), key);
}

void bst_insert_local(NODE_LOCAL **tree, char *key, token *tkn) {
  
  if (!*tree){
    (*tree) = malloc(sizeof(NODE_LOCAL));
    if (!(tree)){
	  exit(99);	
      return;
    }
	
	
	(*tree)->key = malloc(sizeof(strlen(key)));
	if ((*tree)->key == NULL){
		exit(99);	
    	return;
    }
	strcpy((*tree) -> key, key);
	
	(*tree) -> left = (*tree) -> right = NULL;

	(*tree) -> token = malloc(sizeof(token));
	if ((*tree)->token == NULL){
		exit(99);	
    	return;
    }
	memcpy((*tree) -> token, tkn, sizeof(token));

  }
	if (strcmp((*tree)->key, key) == 0) 
	{
		//error();
		return;
	}

	if (strcmp((*tree)->key, key) < 0) 
	{
		bst_insert_local(&((*tree) -> left), key, tkn);
		return;
	}
	bst_insert_local(&((*tree)->right), key, tkn);
}

void bst_dispose(NODE **tree) {
  if (!*tree)
	{
		return;
	}

	bst_dispose(&((*tree)->left));
	bst_dispose(&((*tree)->right));


	bst_dispose_local(&((*tree)->local));
	//free((*tree)->params_names);
	for(int i = 0; i < 32; i++)
	{
		if((*tree)->params_names[i] != NULL){
			free((*tree)->params_names[i]);
		}
	}
	free((*tree)->key);
	free(*tree);
	*tree = NULL;
} 

void bst_dispose_local(NODE_LOCAL **tree)
{
	 if (!*tree)
	{
		return;
	}

	bst_dispose_local(&((*tree)->left));
	bst_dispose_local(&((*tree)->right));
	//free((*tree)->token->name);
	//free((*tree)->token->string);
	//free((*tree)->token->name);
	free((*tree)->key);
	free((*tree)->token);
	free(*tree);
	*tree = NULL;
}

void bst_addtype(char *key, NODE **tree, int type)
{
	NODE *node = bst_search(*tree, key);
	if (node == NULL){
		exit(3);
		return;
	}
	node->return_type = type;
}

void bst_insert_return_type(NODE *tree, int type)
{
	if (tree == NULL){

		exit(3);
		return;
	}
	tree->return_type = type;
}

void bst_update_type_local(char *key_func, char *variable , NODE **tree, int type)
{
	(void)key_func;
	NODE *node = bst_search(*tree, variable);
	if (node == NULL){
		exit(3);
		return;
	}

	if(bst_search_local(node->local, variable) == NULL){
		//exit();
		return;
	}
	bst_search_local(node->local, variable)->type = type;
}

void bst_update_type_local_func(char *key_func, char *variable , NODE **tree, int type)
{
	NODE *node = bst_search(*tree, key_func);
	if (node == NULL){
		exit(3);
		return;
	}

	if(bst_search_local(node->local, variable) == NULL){
		exit(5);
		return;
	}
	bst_search_local(node->local, variable)->type = type;
}

//@bst_insert_built_in = insert built in functions
void bst_insert_built_in(NODE **tree){
	bst_insert(tree, "reads");
	bst_insert(tree, "readi");
	bst_insert(tree, "readf");
	bst_insert(tree, "strlen");
	bst_insert(tree, "substring");
	bst_insert(tree, "ord");
	bst_insert(tree, "chr");
	NODE *node = bst_search(*tree, "reads");
	if(!node){exit(99); return;}
	node -> return_type = T_STRING;
	node -> param_count = 0;
	node -> return_type_is_q = true;
	node = bst_search(*tree, "readi");
	if(!node){exit(99); return;}
	node -> return_type = T_INT;
	node -> param_count = 0;
	node -> return_type_is_q = true;
	node = bst_search(*tree, "readf");
	if(!node){exit(99); return;}
	node -> return_type = T_FLOAT;
	node -> param_count = 0;
	node -> return_type_is_q = true;
	node = bst_search(*tree, "strlen");
	if(!node){exit(99); return;}
	node -> return_type = T_INT;
	node -> param_count = 1;
	node -> return_type_is_q = false;
	node -> params[0] = T_STRING;
	node = bst_search(*tree, "substring");
	if(!node){exit(99); return;}
	node -> return_type = T_STRING;
	node -> return_type_is_q = true;
	node -> param_count = 3;
	node -> params[0] = T_STRING;
	node -> params[1] = T_INT;
	node -> params[2] = T_INT;
	node = bst_search(*tree, "ord");
	if(!node){exit(99); return;}
	node -> return_type = T_INT;
	node -> param_count = 1;
	node -> return_type_is_q = false;
	node -> params[0] = T_STRING;
	node = bst_search(*tree, "chr");
	if(!node){exit(99); return;}
	node -> return_type = T_STRING;
	node -> param_count = 1;
	node -> return_type_is_q = false;
	node -> params[0] = T_INT;
	return;
}
