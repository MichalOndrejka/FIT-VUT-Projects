/* ********************** Precedent_analysis.h *************************** */
/* ********** Implemetácia prekladaču imperatívneho jazyka IFJ22 ********* */
/*  Predmet: Formálne jazyky a prekladače (IFJ) - FIT VUT v Brne           */
/*  Team: xondre15, xtruch01, xbrnak01, xhrach06                           */
/*  Vytvoril: xondre15                                                     */
/* *********************************************************************** */
#pragma once
#include "Lexical_analysis.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtable.c"
#define MAXSTACK 100

int varIndex = 1;
int top = -1;

//Type of the item in stack
typedef enum {
    OPERAND,
    OPERATION,
    LEFTBRACKET,
    RIGHTBRACKET,
    LEFTPRECEDENCE,
    RIGHTPRECEDENCE,
    PEQUALS,
    DOLLAR,
    EXPRESSION,
    ERROR,
} ItemType;


//Expression type
typedef enum {
    PNULL,
    PINT,
    PFLOAT,
    PBOOL,
    PSTRING,
    INVALID
} ExpressionType;


//Struct for stack's items
typedef struct{
    ItemType itemType;

    ExpressionType expressionType;

    Lexeme lex;
} StackItem;

StackItem stack[MAXSTACK];

//Precdent table
char precedentTable[10][10] = {
    //1.: E -> <E*E>
    //2.: E -> <E/E>
    //3.: E -> <E+E>
    //4.: E -> <E-E>
    //5.: E -> <E.E>
    //6.: E -> <(E)>
    //7.: E -> <ErelE>
    //8.: E -> <i>
    //[*]  [/]  [+]  [-]  [.]  [(]  [)]  [i] ,[rel]   [$] <-input
     {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'<' ,'>'    ,'>'}, //[*]
     {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'<' ,'>'    ,'>'}, //[/]
     {'<' ,'<' ,'>' ,'>' ,'>' ,'<' ,'>' ,'<' ,'>'    ,'>'}, //[+]
     {'<' ,'<' ,'>' ,'>' ,'>' ,'<' ,'>' ,'<' ,'>'    ,'>'}, //[-]
     {'<' ,'<' ,'>' ,'>' ,'>' ,'<' ,'>' ,'<' ,'>'    ,'>'}, //[.] <-stack
     {'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'=' ,'<' ,'<'    ,'-'}, //[(]
     {'>' ,'>' ,'>' ,'>' ,'>' ,'-' ,'>' ,'-' ,'>'    ,'>'}, //[)]
     {'>' ,'>' ,'>' ,'>' ,'>' ,'-' ,'>' ,'-' ,'>'    ,'>'}, //[i]
     {'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'>' ,'<' ,'>'    ,'>'}, //[rel]
     {'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'-' ,'<' ,'<'    ,'-'}  //[$]
};


//signatures of functions for precedence analysis
void syntaxError(Lexeme lex);
void semanticTypeCompatibilityError(Lexeme lex);
void semanticError(Lexeme lex);
void internError();
void printStack();
void push(StackItem lex);
void pop();
bool isEmpty();
bool isFull();
StackItem peek(int indexFromTop);
StackItem getPrecedence(StackItem inputLex);
void operationRule();
void bracketsRule();
void operandRule(char *fun, NODE *tree);
void handlePrecedence(StackItem lexItem);
ExpressionType checkRules(StackItem lexItem, char *fun, NODE *tree);
ItemType getItemType(Lexeme lex);
StackItem createLexItem(Lexeme lex);
ExpressionType handleStackItem(StackItem lexItem, char *fun, NODE *tree);
void rule_expression(Lexeme lex, char *fun, NODE *tree);
void startExpression();
ExpressionType endExpression(char *fun, NODE *tree);