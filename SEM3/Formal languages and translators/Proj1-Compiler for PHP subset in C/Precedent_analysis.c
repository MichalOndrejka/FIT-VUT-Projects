/* *********************** Precedent_analysis.c ************************** */
/* ********** Implemetácia prekladaču imperatívneho jazyka IFJ22 ********* */
/*  Predmet: Formálne jazyky a prekladače (IFJ) - FIT VUT v Brne           */
/*  Team: xondre15, xtruch01, xbrnak01, xhrach06                           */
/*  Vytvoril: xondre15                                                     */
/* *********************************************************************** */
#pragma once
#include "Precedent_analysis.h"
//Struct for stack's items
/**
 * @brief Print Syntax error to stdout and exit(2)
 */
void syntaxError(Lexeme lex) {
    if (lex.lineNumber == -1) {
        fprintf(stderr, "Precedent_analysis: Syntax error on line ¯\\_(ツ)_/¯\n");
    } else {
        fprintf(stderr, "Precedent_analysis: Syntax error on line %d\n", lex.lineNumber);
    }
    exit(2);
}

/**
 * @brief Print Type compatibility error to stdout and exit(7)
 */
void semanticTypeCompatibilityError(Lexeme lex) {
    fprintf(stderr, "Precedent_analysis: Semantic type compatibility error on line %d\n", lex.lineNumber);
    exit(7);
}

/**
 * @brief Print Semantic error to stdout and exit(8)
 */
void semanticError(Lexeme lex) {
    printStack();
    printf("lex: %s\n", str_lexeme(lex));
    fprintf(stderr, "Precedent_analysis: Other semantic error %d\n", lex.lineNumber);
    exit(8);
}

/**
 * @brief exit(99)
 */ 
void internError() {
    exit(99);
}

/**
 * @brief Print items in stack from left to right, right is the topmost item in stack
 */
void printStack() {
    for (int i = 0; i <= top; i++) {
        if (stack[i].itemType == DOLLAR) printf("$ ");
        else if (stack[i].itemType == EXPRESSION) printf("E ");
        else if (stack[i].itemType == LEFTPRECEDENCE) printf("< ");
        else if (stack[i].itemType == RIGHTPRECEDENCE) printf("> ");
        else if (stack[i].itemType == PEQUALS) printf("= ");
        else printf("'%s' ",str_lexeme(stack[i].lex));
    }
    printf("\n");
}

/**
 * @brief Push lex param to the stack
 * 
 * @param lex: stack item to be pushed
 */
void push(StackItem lex){
    if(top == MAXSTACK-1) {
        //printf("Precedent_analysis: Overflow State - can't add more elements into the stack\n");
        internError();
    }
    else{
        top+=1;
        stack[top] = lex;
    }
}

/**
 * @brief Pop top of the stack
 */
void pop(){
    if(top == -1) {
        //printf("Precedent_analysis: Underflow State - stack already empty, can't remove any element\n");
        internError();
    }
    else{
        top-=1;
    }
}

/**
 * @brief Check if stack is empty
 * 
 * @return true if stack is empty, false otherwise
 */
bool isEmpty(){
    return top == -1;
}

/**
 * @brief Check if stack is full
 * 
 * @return true if stack is full, false otherwise
 */
bool isFull(){
    return top == MAXSTACK-1;
}

/**
 * @brief Get StackItem from stack based on the indexFromTop
 * 
 * @param indexFromTop: index the stack from the top (0 is top)
 * 
 * @return StackItem on the index from top
 */
StackItem peek(int indexFromTop){
    StackItem item;
    item.itemType = ERROR;
    if (!isEmpty()) {
        indexFromTop = top - indexFromTop;
        if (0 <= indexFromTop && indexFromTop <= top) item = stack[indexFromTop];
    }
    return item;
}
/**
 * @brief Get precedence of inputLex and top of the stack item, 
 * if there is an expression on the top it uses one StackItem before 
 * 
 * @param inputLex: input Lexeme used for getting precedence
 * 
 * @return StackItem precdence
 */
StackItem getPrecedence(StackItem inputLex){
    int index = 0;
    if (peek(index).itemType == EXPRESSION) index = 1;

    StackItem precedence;
    precedence.itemType = ERROR;

    int column;
    int row;
    
    if (inputLex.itemType == DOLLAR) {
        column = 9;
    } else {
        switch (inputLex.lex.kind) {
        case MULTIPLICATION: column = 0; break;
        case DIVISION: column = 1; break;
        case ADDITION: column = 2; break;
        case SUBSTRACTION: column = 3; break;
        case DOT: column = 4; break;
        case LBRACKET: column = 5; break;
        case RBRACKET: column = 6; break;
        case KEYWORD: column = 7; break;
        case VAR: column = 7; break;
        case INT: column = 7; break;
        case FLOATA: column = 7; break;
        case FLOATB: column = 7; break;
        case STRING: column = 7; break;
        case EQUALS: column = 8; break;
        case NOTEQ: column = 8; break;
        case LESS: column = 8; break;
        case LESSEQ: column = 8; break;
        case GREATER: column = 8; break;
        case GREATEREQ: column = 8; break;
        default: column = -1; break;
        }
    }

    if (peek(index).itemType == DOLLAR) {
        row = 9;
    } else {
        switch (peek(index).lex.kind) {
        case MULTIPLICATION: row = 0; break;
        case DIVISION: row = 1; break;
        case ADDITION: row = 2; break;
        case SUBSTRACTION: row = 3; break;
        case DOT: row = 4; break;
        case LBRACKET: row = 5; break;
        case RBRACKET: row = 6; break;
        case KEYWORD: row = 7; break;
        case VAR: row = 7; break;
        case INT: row = 7; break;
        case FLOATA: row = 7; break;
        case FLOATB: row = 7; break;
        case STRING: row = 7; break;
        case EQUALS: row = 8; break;
        case NOTEQ: row = 8; break;
        case LESS: row = 8; break;
        case LESSEQ: row = 8; break;
        case GREATER: row = 8; break;
        case GREATEREQ: row = 8; break;
        default: row = -1; break;
        }
    }
    if (column == -1 || row == -1) {
        printf("Precedent_analysis: Couldn't get precedence of the current input\n");
        syntaxError(inputLex.lex);
    } else {
        switch (precedentTable[row][column]) {
            case '<': precedence.itemType = LEFTPRECEDENCE; break;
            case '>': precedence.itemType = RIGHTPRECEDENCE; break;
            case '=': precedence.itemType = PEQUALS; break;
            default: precedence.itemType = ERROR; break;
    }
    }
    return precedence;
}

/**
 * @brief Based on operation between expressions handle the specific
 * operation (pop precedences in stack evaluate expressions and replace 
 * them with only one expression.
 * <E*E> -> E, 
 * <E/E> -> E, 
 * <E+E> -> E, 
 * <E-E> -> E, 
 * <E.E> -> E,
 * <ErelE> -> E
 */
void operationRule() {
    StackItem item;
    item.itemType = EXPRESSION;
    StackItem leftOP = peek(3);
    StackItem rightOP = peek(1);

    switch (peek(2).lex.kind)
    {
    case MULTIPLICATION:
        if (leftOP.expressionType != PINT && leftOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);
        else if (rightOP.expressionType != PINT && rightOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);

        if (leftOP.expressionType == PINT && rightOP.expressionType == PFLOAT) { 
            item.expressionType = PFLOAT;               
            printf("POPS GF@var1\n");
            printf("POPS GF@var2\n");
            printf("INT2FLOAT GF@var2 GF@var2\n");
            printf("MUL GF@var1 GF@var1 GF@var2\n");
            printf("PUSHS GF@var1\n");
        } else if (rightOP.expressionType == PINT && leftOP.expressionType == PFLOAT) {
            item.expressionType = PFLOAT; 
            printf("POPS GF@var1\n");
            printf("POPS GF@var2\n");
            printf("INT2FLOAT GF@var1 GF@var1\n");
            printf("MUL GF@var1 GF@var1 GF@var2\n");
            printf("PUSHS GF@var1\n");
        } else {
            if (rightOP.expressionType == PINT) item.expressionType = PINT;
            else item.expressionType = PFLOAT; 
            printf("MULS\n");
        }
        break;
    case DIVISION:
        if (leftOP.expressionType != PINT && leftOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);
        else if (rightOP.expressionType != PINT && rightOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);

        if (rightOP.expressionType == PINT && rightOP.lex.value == 0) semanticError(peek(2).lex);
        else if (rightOP.expressionType == PFLOAT && rightOP.lex.fvalue == 0.0) semanticError(peek(2).lex);

        item.expressionType = PFLOAT; 
        if (leftOP.expressionType == PINT && rightOP.expressionType == PFLOAT) {            
            printf("POPS GF@var1\n");
            printf("POPS GF@var2\n");
            printf("INT2FLOAT GF@var2 GF@var2\n");
            printf("DIV GF@var1 GF@var2 GF@var1\n");
            printf("PUSHS GF@var1\n");
        } else if (leftOP.expressionType == PFLOAT && rightOP.expressionType == PINT) {
            printf("POPS GF@var1\n");
            printf("POPS GF@var2\n");
            printf("INT2FLOAT GF@var1 GF@var1\n");
            printf("DIV GF@var1 GF@var2 GF@var1\n");
            printf("PUSHS GF@var1\n");
        } else if (leftOP.expressionType == PINT && rightOP.expressionType == PINT) {
            printf("POPS GF@var1\n");
            printf("POPS GF@var2\n");
            printf("INT2FLOAT GF@var1 GF@var1\n");
            printf("INT2FLOAT GF@var2 GF@var2\n");
            printf("DIV GF@var1 GF@var2 GF@var1\n");
            printf("PUSHS GF@var1\n");
        } else printf("DIVS\n");
        break;
    case ADDITION:
        if (leftOP.expressionType != PINT && leftOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);
        else if (rightOP.expressionType != PINT && rightOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);

        if (leftOP.expressionType == PINT && rightOP.expressionType == PFLOAT) { 
            item.expressionType = PFLOAT;               
            printf("POPS GF@var1\n");
            printf("POPS GF@var2\n");
            printf("INT2FLOAT GF@var2 GF@var2\n");
            printf("ADD GF@var1 GF@var1 GF@var2\n");
            printf("PUSHS GF@var1\n");
        } else if (leftOP.expressionType == PFLOAT && rightOP.expressionType == PINT) {
            item.expressionType = PFLOAT; 
            printf("POPS GF@var1\n");
            printf("POPS GF@var2\n");
            printf("INT2FLOAT GF@var1 GF@var1\n");
            printf("ADD GF@var1 GF@var1 GF@var2\n");
            printf("PUSHS GF@var1\n");
        } else {
            if (rightOP.expressionType == PINT) item.expressionType = PINT;
            else item.expressionType = PFLOAT; 
            printf("ADDS\n");
        }
        break;
    case SUBSTRACTION:
        if (leftOP.expressionType != PINT && leftOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);
        else if (rightOP.expressionType != PINT && rightOP.expressionType != PFLOAT) semanticTypeCompatibilityError(peek(2).lex);

        if (leftOP.expressionType == PINT && rightOP.expressionType == PFLOAT) { 
                item.expressionType = PFLOAT;               
                printf("POPS GF@var1\n");
                printf("POPS GF@var2\n");
                printf("INT2FLOAT GF@var2 GF@var2\n");
                printf("SUB GF@var1 GF@var2 GF@var1\n");
                printf("PUSHS GF@var1\n");
            } else if (rightOP.expressionType == PINT && leftOP.expressionType == PFLOAT) {
                item.expressionType = PFLOAT; 
                printf("POPS GF@var1\n");
                printf("POPS GF@var2\n");
                printf("INT2FLOAT GF@var1 GF@var1\n");
                printf("SUB GF@var1 GF@var2 GF@var1\n");
                printf("PUSHS GF@var1\n");
            } else {
                if (rightOP.expressionType == PINT) item.expressionType = PINT;
                else item.expressionType = PFLOAT; 
                printf("SUBS\n");
            }
        break;
    case DOT:
        if (rightOP.expressionType != PSTRING || leftOP.expressionType != PSTRING ) semanticTypeCompatibilityError(peek(2).lex);
        item.expressionType = PSTRING;
        printf("POPS GF@var1\n");
        printf("POPS GF@var2\n");
        if (leftOP.expressionType == PNULL) printf("MOVE GF@var2 string@\n"); //do var2 daj ""
        else if (leftOP.expressionType == PBOOL) printf("MOVE GF@var2 string@\n"); //do var2 daj ""
        if (rightOP.expressionType == PNULL) printf("MOVE GF@var1 string@\n"); //do var1 daj ""
        else if (rightOP.expressionType == PBOOL) printf("MOVE GF@var1 string@\n"); //do var1 daj ""
        printf("CONCAT GF@var1 GF@var1 GF@var2\n");
        printf("PUSHS GF@var1\n");
        break;
    case EQUALS:
        if (rightOP.expressionType != leftOP.expressionType) semanticTypeCompatibilityError(peek(2).lex);
        item.expressionType = PBOOL;
        printf("POPS GF@var1\n");
        printf("POPS GF@var2\n");
        printf("EQ GF@var1 GF@var1 GF@var2\n");
        printf("PUSHS GF@var1\n");
        break;
    case NOTEQ:
        if (rightOP.expressionType != leftOP.expressionType) semanticTypeCompatibilityError(peek(2).lex);
        item.expressionType = PBOOL;
        printf("POPS GF@var1\n");
        printf("POPS GF@var2\n");
        printf("EQ GF@var1 GF@var1 GF@var2\n");
        printf("NOT GF@var1 GF@var1\n");
        printf("PUSHS GF@var1\n");
        break;
    case LESS:
        if (rightOP.expressionType != leftOP.expressionType) semanticTypeCompatibilityError(peek(2).lex);
        item.expressionType = PBOOL;
        printf("POPS GF@var1\n");
        printf("POPS GF@var2\n");
        printf("LT GF@var1 GF@var2 GF@var1\n");
        printf("PUSHS GF@var1\n");
        break;
    case LESSEQ:
        if (rightOP.expressionType != leftOP.expressionType) semanticTypeCompatibilityError(peek(2).lex);
        item.expressionType = PBOOL;
        printf("POPS GF@var1\n");
        printf("POPS GF@var2\n");
        printf("LT GF@var3 GF@var2 GF@var1\n");
        printf("EQ GF@var4 GF@var1 GF@var2\n");
        printf("OR GF@var1 GF@var3 GF@var4\n");
        printf("PUSHS GF@var1\n");
        break;
    case GREATER:
        if (rightOP.expressionType != leftOP.expressionType) semanticTypeCompatibilityError(peek(2).lex);
        item.expressionType = PBOOL;
        printf("POPS GF@var1\n");
        printf("POPS GF@var2\n");
        printf("GT GF@var1 GF@var2 GF@var1\n");
        printf("PUSHS GF@var1\n");
        break;
    case GREATEREQ:
    if (rightOP.expressionType != leftOP.expressionType) semanticTypeCompatibilityError(peek(2).lex);
        item.expressionType = PBOOL;
        printf("POPS GF@var1\n");
        printf("POPS GF@var2\n");
        printf("GT GF@var3 GF@var2 GF@var1\n");
        printf("EQ GF@var4 GF@var1 GF@var2\n");
        printf("OR GF@var1 GF@var3 GF@var4\n");
        printf("PUSHS GF@var1\n");
        break;
    default:
        printf("Precedent_analysis: Invalid operantion error\n");
        syntaxError(peek(2).lex);
        break;
    }
    pop();
    pop();
    pop();
    pop();
    pop();
    push(item);
}

/**
 * @brief Handle the brackets rule (pop precedences and replace 
 * brackets and expression with only one expression)
 * <(E)> -> E
 */
void bracketsRule() {
    StackItem item = peek(2); //
    pop();
    pop();
    pop();
    pop();
    pop();
    push(item);
}

/**
 * @brief Handle the operand rule (pop precedences and replace 
 * var,str,int or float with expression of that type)
 * <i> -> E
 */
void operandRule(char *fun, NODE *tree) {
    StackItem expression = peek(1);
    expression.itemType = EXPRESSION;
    switch (expression.lex.kind) {
        case VAR: ; 
            NODE *current = bst_search(tree, fun);
            token *current_var = bst_search_local(current->local, expression.lex.string); 
            if(current_var == NULL){exit(5);}
            switch (current_var->type)
            {
            case T_INT:
                expression.expressionType = PINT;
                break;
            case T_FLOAT:
                expression.expressionType = PFLOAT;
                break;
            case T_STRING:
                expression.expressionType = PSTRING;
                break;
            default:
                expression.expressionType = PNULL;
                break;
            }
           //printf("MOVE GF@var%d LF@%s\n", varIndex, expression.lex.string);
            printf("PUSHS LF@%s\n",expression.lex.string);
            break;
        case INT: 
            printf("PUSHS int@%d\n", expression.lex.value);
            expression.expressionType = PINT;
            break;
        case STRING: 
            //printf("%c\n", expression.lex.string[strlen(expression.lex.string) - 1]);
            expression.lex.string[strlen(expression.lex.string) - 1] = '\0';
            expression.lex.string++; 
            printf("PUSHS string@%s\n",expression.lex.string);
            varIndex ++;   // niekde treba varIndex--

            expression.expressionType = PSTRING;
            break;
        case FLOATA: 
            printf("PUSHS float@%a\n", expression.lex.fvalue);
            varIndex ++;   // niekde treba varIndex--
            
            expression.expressionType = PFLOAT;
            break;
        case FLOATB: 
            printf("PUSHS float@%a\n", expression.lex.fvalue);
            varIndex ++;   // niekde treba varIndex--
            expression.expressionType = PFLOAT;
            break;
        case KEYWORD:
            printf("PUSHS nil@nil\n");
            varIndex ++;   // niekde treba varIndex--
           
            expression.expressionType = PNULL;
            break;
        default: 
            syntaxError(expression.lex);
            break;
    }
    pop();
    pop();
    pop();
    push(expression);
}

/**
 * @brief Push precedence to the stack.  
 * If Precdence is EQUALS don't do anything,
 * else if pushing LeftPrecedence and Expression is on top of the
 * stack push before Expression, else just push Precedence
 * 
 * @param lexItem Precedence of this item is being pushed
 */
void handlePrecedence(StackItem lexItem) { 
    StackItem precedence = getPrecedence(lexItem);

    if (precedence.itemType == LEFTPRECEDENCE) {

        if (peek(0).itemType == EXPRESSION) {
            StackItem expression = peek(0);
            pop();
            push(precedence);
            push(expression);
        } else if (peek(0).itemType == DOLLAR) push(precedence);
        else if (peek(0).itemType == OPERATION) push(precedence);
        else if (peek(0).itemType == LEFTBRACKET) push(precedence);
        else syntaxError(lexItem.lex);

    } else if (precedence.itemType == RIGHTPRECEDENCE) {

        if (peek(0).itemType == OPERAND) push(precedence);
        else if (peek(0).itemType == EXPRESSION) push(precedence);
        else if (peek(0).itemType == RIGHTBRACKET) push(precedence);
        else syntaxError(lexItem.lex);

    } else if (precedence.itemType == ERROR) syntaxError(lexItem.lex);
}

/**
 * @brief Check items on the top of the stack, if there is any rule, 
 * call specific function to handle given rule
 * 
 * @param lexItem: is used to get a new precedence in case a rule is applied
 * 
 * @return ExpressionType INVALID if the expression is yet invalid, any valid ExpressionType otherwise
 */
ExpressionType checkRules(StackItem lexItem, char *fun, NODE *tree) {
    bool apppliedRule = false;

    if (peek(0).itemType == RIGHTPRECEDENCE) {
        if (peek(1).itemType == OPERAND && peek(2).itemType == LEFTPRECEDENCE) {
            apppliedRule = true;
            operandRule(fun, tree);

        } else if (peek(4).itemType == LEFTPRECEDENCE) {
            if (peek(1).itemType == EXPRESSION && peek(3).itemType == EXPRESSION) {
                if (peek(2).itemType == OPERATION) {
                    apppliedRule = true;
                    operationRule();
                } else syntaxError(lexItem.lex);

            } else if (peek(1).itemType == RIGHTBRACKET && peek(3).itemType == LEFTBRACKET) {
                if (peek(2).itemType == EXPRESSION) {
                    apppliedRule = true;
                    bracketsRule();
                } else syntaxError(lexItem.lex);

            } else syntaxError(lexItem.lex);
        } else syntaxError(lexItem.lex);
    }

    if (peek(0).itemType == EXPRESSION && peek(1).itemType == DOLLAR && lexItem.itemType == DOLLAR) {
        return peek(0).expressionType;
    }

    if (apppliedRule) {
        handlePrecedence(lexItem);
        return checkRules(lexItem, fun, tree);
    } else {
        return INVALID;
    }
}

/**
 * @brief Get the type of based on Lexeme kind
 * 
 * @param lex: get item type of this param
 * 
 * @return ItemType of lex, in case of invalid lex return ERROR ItemType
 */
ItemType getItemType(Lexeme lex) {
    switch (lex.kind)
    {
    case MULTIPLICATION: return OPERATION;
    case DIVISION: return OPERATION;
    case ADDITION: return OPERATION;
    case SUBSTRACTION: return OPERATION;
    case DOT: return OPERATION;
    case EQUALS: return OPERATION;
    case NOTEQ: return OPERATION;
    case LESS: return OPERATION;
    case LESSEQ: return OPERATION;
    case GREATER: return OPERATION;
    case GREATEREQ: return OPERATION;
    case VAR: return OPERAND;
    case INT: return OPERAND;
    case STRING: return OPERAND;
    case FLOATA: return OPERAND;
    case FLOATB: return OPERAND;
    case LBRACKET: return LEFTBRACKET;
    case RBRACKET: return RIGHTBRACKET;
    case KEYWORD: 
        if (strcmp(lex.string, "null") != 0) semanticError(lex);
        return OPERAND;
    default:
        syntaxError(lex);
        return ERROR;
        break;
    }
}

/**
 * @brief Create StackItem from lex. Save itemType to it's attributes
 * 
 * @param lex: Lexeme which is used for creating StackItem
 * 
 * @return StackItem with it's attributes
 */
StackItem createLexItem(Lexeme lex) {
    StackItem lexItem;
    lexItem.itemType = getItemType(lex);
    lexItem.lex = lex;
    return lexItem;
}

/**
 * @brief Handle precedence of StackItem, check for rules and push the StackItem to the stack
 * 
 * @param lexItem: lexItem to be handled
 * 
 * @return ExpressionType INVALID if the expression is yet invalid, any valid ExpressionType otherwise
 */
ExpressionType handleStackItem(StackItem lexItem, char *fun, NODE *tree) {
    handlePrecedence(lexItem);

    ExpressionType temp = checkRules(lexItem, fun, tree);
    if (temp == INVALID) {
        push(lexItem);
        return INVALID;
    } else {
        return temp;
    }
}

/**
 * @brief Create StackItem from Lexeme and hadnle it's precedence
 * 
 * @param lex: Lexeme from which StackItem is being created
 */
void rule_expression(Lexeme lex, char *fun, NODE *tree) {
    StackItem lexItem = createLexItem(lex);
    handleStackItem(lexItem, fun, tree);
}

/**
 * @brief Clear stack and push StackItem DOLLAR (Start of expression) to the top of the stack
 */
void startExpression() {
    top = -1;
    StackItem startItem;
    startItem.lex.lineNumber = -1;
    startItem.itemType = DOLLAR;

    push(startItem);
}

/**
 * @brief call handleStackItem with param DOLLAR (End of expression)
 * 
 * @return ExpressionType INVALID if the expression is invalid, any valid ExpressionType otherwise
 */
ExpressionType endExpression(char *fun, NODE *tree) {
    StackItem endItem;
    endItem.itemType = DOLLAR;

    ExpressionType temp = handleStackItem(endItem, fun, tree);
    varIndex = 1;
    return temp;
}