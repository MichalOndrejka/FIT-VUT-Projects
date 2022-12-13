/* ************************ Lexical_analysis.c *************************** */
/* ********** Implemetácia prekladaču imperatívneho jazyka IFJ22 ********* */
/*  Predmet: Formálne jazyky a prekladače (IFJ) - FIT VUT v Brne           */
/*  Team: xondre15, xtruch01, xbrnak01, xhrach06                           */
/*  Vytvoril: xtruch01                                                     */
/* *********************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
//enumerator na stavy konecneho automatu
typedef enum {
    Start,

    Dot,
    Comma,
    Semicolon,
    Colon,

    LBracket,
    RBracket,
    LCurly,
    RCurly,

    Var1,
    Var,

    StringA,
    String,
    Backslash,
    HexA,
    HexB,
    EscapeA,
    EscapeB,

    Int,
    Float,
    Float1,
    Float2,
    Float3,
    FloatA,
    FloatB,

    Division,
    CommentA,
    CommentB,
    CommentC,
    Comment,
    Addition,
    Substraction,
    Multiplication,
    Assignment,
    EqualsA,
    Equals,
    Not,
    NotEqA,
    NotEq,

    QuestionMark,
    End,
    Id,

    Less,
    Greater,
    LessEq,
    GreaterEq,

    PrologueA,
    PrologueB,
    PrologueC,
    Prologue,
    Error,
    Next,
} AutomatState;

typedef enum{
        STRING,
        ASSIGNMENT,
        EQUALS,
        NOTEQ,
        NOT,
        RCURLY,
        LCURLY,
        RBRACKET,
        LBRACKET,
        GREATER,
        GREATEREQ,
        LESS,
        LESSEQ,
        PROLOGUE,
        SUBSTRACTION,
        ADDITION,
        MULTIPLICATION,
        DIVISION,
        COMMENT,
        VAR,
        INT,
        FLOATA,
        FLOATB,
        ID,
        KEYWORD,
        END,
        DOT,
        COMMA,
        COLON,
        SEMICOLON,    
        LEOF,
    } Kind;
//struktura Lexem
typedef struct{
    Kind kind;
    char *string;
    int value;
    float fvalue;
    int lineNumber;
    } Lexeme;

#define EOL '\n'
char *tokenString = NULL;
int lineNumber = 1;
bool skipEnd = false;

int isunderscore(char in);
int isAssignment(char in);
int isquote(char in);
int isbackslash(char in);
int isnumberinrange(char in, int downrange, int uprange);
AutomatState getState(AutomatState currentState, char in);
Lexeme makeLexeme(AutomatState finalState, char *Token);
Lexeme getLexeme1();
char* str_lexeme(Lexeme in);
Lexeme getLexeme();