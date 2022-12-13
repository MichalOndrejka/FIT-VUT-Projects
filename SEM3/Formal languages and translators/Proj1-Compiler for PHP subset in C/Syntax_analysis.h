/* ************************ Syntax_analysis.h **************************** */
/* ********** Implemetácia prekladaču imperatívneho jazyka IFJ22 ********* */
/*  Predmet: Formálne jazyky a prekladače (IFJ) - FIT VUT v Brne           */
/*  Team: xondre15, xtruch01, xbrnak01, xhrach06                           */
/*  Vytvoril: xhrach06, xbrnak01                                           */
/* *********************************************************************** */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Lexical_analysis.c"
#include "symtable.c"
#include "Precedent_analysis.c"

//global variables
// @fun store function name we are in
// @curr_fun stores current function
// @tree stores symbol table
// @param_def_count count number of parameters in function definition
// @param_in_count count number of parameters in function call
// @assign_function stores token name in assignement
// @in_function is true if we are in function
// @exit_function is true if we are in function and we are in return statement
// @Token stores token

bool exit_without_return;
bool exit_return;
bool in_function = false;
bool exit_function = false;
token *Token;
char *fun;
NODE *tree;
int param_def_count = 0;
int param_in_count=0;
token *assign_function;
Lexeme lex;
int in_condition = 0;
int label_count = 0;
bool exit_condition = false;
int in_while = 0;
int while_count = 0;

//signatures of functions for syntax analysis
void clear_all();
void clear_token();
void type_copy(Lexeme lex);
type type_copy_add(Lexeme lex);
void rule_prologue();
void rule_p_body();
void rule_p_prikaz();
void rule_p_end();
void rule_assignement(Lexeme VariableLex);
void rule_func_call(char *return_to);
void rule_func_typesq();
void rule_func_body();
void rule_p_prikaz();
void rule_params(Lexeme lex);
void rule_types();
void rule_params(Lexeme lex);
void rule_params_in(Lexeme lex, char *return_to);
void rule_ret_value();
void writeGenerate(Lexeme lex, char *return_to);
void p_start();