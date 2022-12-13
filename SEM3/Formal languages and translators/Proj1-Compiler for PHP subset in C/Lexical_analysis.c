/* ************************ Lexical_analysis.c *************************** */
/* ********** Implemetácia prekladaču imperatívneho jazyka IFJ22 ********* */
/*  Predmet: Formálne jazyky a prekladače (IFJ) - FIT VUT v Brne           */
/*  Team: xondre15, xtruch01, xbrnak01, xhrach06                           */
/*  Vytvoril: xtruch01                                                     */
/* *********************************************************************** */

#pragma once
#include "Lexical_analysis.h"

int isunderscore(char in){
    return (in == '_');
}
int isAssignment(char in){
    return(in == '=');
}
int isquote(char in){
    return(in == '"');
}
int isbackslash(char in){
    return (in == '\\');
}
int isnumberinrange(char in, int downrange, int uprange){
    int v = atoi(&in);
    if (v <= uprange && v >= downrange){
        return 1;
    }
    return 0;
}
//funckia na ziskanie stavu
AutomatState getState(AutomatState currentState, char in)
{
    switch (currentState) {
    case Next:
    case Error:
        fprintf(stderr, "Lexical_analysis: Invalid lexeme on line %d\n", lineNumber);
        exit(1);

    case Start:
        if (in == EOL){
            lineNumber += 1;
            return Start;
        }
        if ((in == '\t') || (in == ' ')){
            return Start;
        }
        if (in == ';'){
            return Semicolon;
        }
        if (in == ':'){
            return Colon;
        }
        if (in == ','){
            return Comma;
        }
        if (in == '.'){
            return Dot;
        }
        if (in == '?'){
            return QuestionMark;
        }
        if (isalpha(in) ||  isunderscore(in)){
            return Id;
        }
        if (in == EOF){
            return End;
        }
        if (in == '(') {
            return LBracket;
        }
        if (in == ')') {
            return RBracket;
        }
        if (in == '{') {
            return LCurly;
        }
        if (in == '}') {
            return RCurly;
        }
        if (in == '$') {
            return Var1;
        }
        if (isquote(in)){
            return StringA;
        }
        if (isdigit(in)) {
            return Int;
        }
        if (in == '/'){
            return Division;
        }
        if (in == '+'){
            return Addition;
        }
        if (in == '-'){
            return Substraction;
        }
        if (in == '*'){
            return Multiplication;
        }
        if (isAssignment(in)){
            return Assignment;
        }
        if (in == '!'){
            return Not;
        }
        if (in == '<'){
            return Less;
        }
        if (in == '>'){
            return Greater;
        }
        return Start;
                                // Variables
    case Var1:
        if (isalpha(in) || isunderscore(in)) {
            return Var;
        }
        return Next;
    case Var:
         if (isalpha(in) || isunderscore(in) || isdigit(in)) {
            return Var;
        }
        return Next;
                                // Strings
    
    case StringA:
        if ((in > 31 && in != 36 && in != 34 && !isbackslash(in)) || in == EOL){
            return StringA;
        }
        if (isbackslash(in)){
            return Backslash;
        }
        if (isquote(in)){
            return String;
        }
        return Error;
    case Backslash:
        if (in == 'x'){
            return HexA;
        }
        if (isquote(in) || in == 'n' || in == 't' || isbackslash(in) || in == '$'){
            return StringA;
        }
        if (isnumberinrange(in, 0, 3)){
            return EscapeA;
        }
        return Error;
    case HexA:
        if ((in >= 'a' && in <='f') || (in >= 'A' && in <='F') || isdigit(in)){
            return HexB;
        }
        return Error;
    case HexB:
        if ((in >= 'a' && in <='f') || (in >= 'A' && in <='F') || isdigit(in)){
            return StringA;
        }
        return Error;
    case EscapeA:
        if (isnumberinrange(in, 0, 7)){
            return EscapeB;
        }
        return Error;
    case EscapeB:
        if (isnumberinrange(in, 1, 7)){
            return StringA;
        }
        return Error;

                                // Numbers
    case Int:
        if (isdigit(in)) {
            return Int;
        }
        if (in == '.') {
            return Float1;
        }
        if (in == 'e' || in == 'E') {
            return Float2;
        }
        return Next;
    case Float1:
        if (isdigit(in)) {
            return FloatA;
        }
        return Error;
    case Float2:
        if (isdigit(in)) {
            return FloatB;
        }
        if (in == '+' || in == '-') {
            return Float3;
        }
        return Error;
    case Float3:
        if (isdigit(in)) {
            return FloatB;
        }
        return Error;
    case FloatA:
        if (isdigit(in)) {
            return FloatA;
        }
        if (in == 'e' || in == 'E') {
            return Float2;
        }
        return Next;
    case FloatB:
        if (isdigit(in)) {
            return FloatB;
        }
        return Next;

                                // Division and comments
    case Division:
        if (in == '/'){
            return CommentA;
        }
        if (in == '*'){
            return CommentB;
        }
        return Next;
    case CommentA: 
        if (in > 31  || in == ' '){
            return CommentA;
        }
        if (in == EOL){
            lineNumber += 1;
            return Comment;
        }
        return Start;
    case CommentB:
        if (in == EOL){
            lineNumber += 1;
        }
        if (in > 31 && in != 42){
            return CommentB;
        }
        if (in == '*'){
            return CommentC;
        }
        return Start;
    case CommentC:
        if (in > 31 && in != '/'){
            return CommentB;
        }
        if (in == '/'){
            return Comment;
        }
        return Start;
    case QuestionMark:
        if (in >= 'a' && in <= 'z'){
            return Id;
        }
        if (in == '>'){
            return End;
        }
        return Error;
    case Id:
        if (isalpha(in) || isdigit(in) || isunderscore(in)){
            return Id;
        }
        return Next;
    case Less:
        if (isAssignment(in)){
            return LessEq;
        }
        if (in == '?'){
            return PrologueA;
        }
        return Next;
    case Greater:
        if(isAssignment(in)){
            return GreaterEq;
        } 
        return Next;
    case Assignment:
        if (isAssignment(in)){
            return EqualsA;
        }
        return Next;
    case EqualsA:
        if (isAssignment(in)){
            return Equals;
        }
        return Error;
    case Not:
        if (isAssignment(in)){
            return NotEqA;
        }
        return Next;
    case NotEqA:
        if (isAssignment(in)){
            return NotEq;
        }
        return Error;
    
    case PrologueA:
        if (in == 'p'){return PrologueB;}
        return Error;
    case PrologueB:
        if (in == 'h'){return PrologueC;}
        return Error;
    case PrologueC:
        if (in == 'p'){return Prologue;}
        return Error;   
    case Comma:
    case Semicolon:
    case Colon:
    case LBracket:
    case RBracket:
    case LCurly:
    case RCurly:
    case String:
    case Float:
    case Comment:
    case Addition:
    case Substraction:
    case Multiplication:
    case Equals:
    case NotEq:
    case End:
    case LessEq:
    case GreaterEq:
    case Prologue:
    case Dot:;
    }
return Next;
}

// Creates Lexeme with all necessary attributes
Lexeme makeLexeme(AutomatState finalState, char *Token){
    switch (finalState)
    {
        case String: return(Lexeme){.kind = STRING, .string = Token, .lineNumber = lineNumber};
        case Assignment: return(Lexeme){.kind = ASSIGNMENT, .lineNumber = lineNumber};
        case Equals: return(Lexeme){.kind = EQUALS, .lineNumber = lineNumber};
        case NotEq: return(Lexeme){.kind = NOTEQ, .lineNumber = lineNumber};
        case Not: return(Lexeme){.kind = NOT, .lineNumber = lineNumber};
        case RCurly: return(Lexeme){.kind = RCURLY, .lineNumber = lineNumber};
        case LCurly: return(Lexeme){.kind = LCURLY, .lineNumber = lineNumber};
        case RBracket: return(Lexeme){.kind = RBRACKET, .lineNumber = lineNumber};
        case LBracket: return(Lexeme){.kind = LBRACKET, .lineNumber = lineNumber};
        case Greater: return(Lexeme){.kind = GREATER, .lineNumber = lineNumber};
        case GreaterEq: return(Lexeme){.kind = GREATEREQ, .lineNumber = lineNumber};
        case Less: return(Lexeme){.kind = LESS, .lineNumber = lineNumber};
        case LessEq: return(Lexeme){.kind = LESSEQ, .lineNumber = lineNumber};
        case Prologue: return(Lexeme){.string = Token, .kind = PROLOGUE, .lineNumber = lineNumber};
        case Substraction: return(Lexeme){.kind = SUBSTRACTION, .lineNumber = lineNumber};
        case Addition: return(Lexeme){.kind = ADDITION, .lineNumber = lineNumber}; 
        case Multiplication: return(Lexeme){.kind = MULTIPLICATION, .lineNumber = lineNumber};
        case Division: return(Lexeme){.kind = DIVISION, .lineNumber = lineNumber};
        case Comment: return(Lexeme){.string = Token, .kind = COMMENT, .lineNumber = lineNumber};
        case Var: return(Lexeme){.kind = VAR, .string = Token, .lineNumber = lineNumber};
        case Int: ;
            int i; 
            sscanf(Token, "%d", &i);
            return(Lexeme){.kind = INT, .string = Token, .value = i, .lineNumber = lineNumber};
        case FloatA: ;
            float fa; 
            sscanf(Token, "%f", &fa);
            return(Lexeme){.kind = FLOATA, .string = Token, .fvalue = fa, .lineNumber = lineNumber};
        case FloatB: ;
            float fb; 
            sscanf(Token, "%f", &fb);
            return(Lexeme){.kind = FLOATB, .string = Token, .fvalue = fb, .lineNumber = lineNumber};
        case Id: ;
                char *keywords[] = {"else", "float", "function", "if",
                "int", "null","?int","?float","?string", "return", "string", "void", "while"};
                for(int i = 0; i < 13; i++){
                    if(!strcmp(keywords[i], Token)){
                        return(Lexeme){.kind = KEYWORD, .string = Token, .lineNumber = lineNumber};
                    }
                }
                return(Lexeme){.kind = ID, .string = Token, .lineNumber = lineNumber};
        case End: return(Lexeme){.kind = END, .lineNumber = lineNumber};
        case Dot: return(Lexeme){.kind = DOT, .lineNumber = lineNumber};
        case Comma: return(Lexeme){.kind = COMMA, .lineNumber = lineNumber};
        case Colon: return(Lexeme){.kind = COLON, .lineNumber = lineNumber};
        case Semicolon: return(Lexeme){.kind = SEMICOLON, .lineNumber = lineNumber};
        case Next: 
            fprintf(stderr, "Lexical_analysis: Invalid lexeme on line %d\n", lineNumber);
            exit(1);
        default: 
            fprintf(stderr, "Lexical_analysis: Invalid lexeme on line %d\n", lineNumber); 
            exit(1);
    }
}

// Get lexemes from FSM
Lexeme getLexeme1(){
    AutomatState current = Start;
    int length = 0;
    while (true)
    {   
        if (length == 0){
            tokenString = NULL;
        }
        int in = getchar();
        if (in == EOF || in == END){
            if ((current == End) && (!skipEnd)){
                skipEnd = true;
                return (Lexeme){.kind = END, .lineNumber = lineNumber};
            }
            else if((current =! End) && (current != Start) && (!skipEnd)){
                skipEnd = true;
                return makeLexeme(current, tokenString);
            }
            else{
                return (Lexeme){.kind = LEOF, .lineNumber = lineNumber};
            }
        }
        AutomatState NextState = getState(current, in);
        if (NextState == Start){
            free(tokenString);
            tokenString = NULL;
            length = 0;
            continue;
        }
        if (NextState != Next){
            if (in != EOL) {
                length += 2;
                tokenString = realloc(tokenString, length);
                if (tokenString == NULL) exit(99);
                char tmpstr[2];
                tmpstr[0] = in;
                tmpstr[1] = '\0';
                
                strcat(tokenString,tmpstr);
            }
        }
    
        if (NextState == Next){
            ungetc(in, stdin);
            length = 0;
            return makeLexeme(current, tokenString);
        }
        current = NextState;
    }

}

// Prints lexemes
char* str_lexeme(Lexeme in){
    switch (in.kind)
    {
    case RCURLY: return "}";
    case LCURLY: return "{";
    case RBRACKET: return ")";
    case LBRACKET: return "(";

    case STRING: return in.string;
    case ASSIGNMENT: return "=";
    case EQUALS: return "===";
    case NOTEQ: return "!==";
    case NOT: return "!";

    case GREATER: return ">";
    case GREATEREQ: return ">=";
    case LESS: return "<";
    case LESSEQ: return "<=";
    case PROLOGUE: return in.string;
    case SUBSTRACTION: return "-";
    case ADDITION: return "+"; 
    case MULTIPLICATION: return"*";
    case DIVISION: return "/";
    case COMMENT: return "some comment";
    case VAR: return in.string;
    case INT: return in.string;
    case FLOATA: return in.string;
    case FLOATB: return in.string;
    case KEYWORD: return in.string;
    case ID: return in.string;                 
    case END: return "?>";
    case DOT: return ".";
    case COMMA: return ",";
    case COLON: return ":";
    case SEMICOLON: return ";";
    case LEOF: return "LEOF";
    default: return("else");
    }
    return ("Error");
}
// Skip comment lexemes
Lexeme getLexeme(){
    Lexeme lex = {0};
    lex = getLexeme1();
    if (lex.kind == COMMENT){
        tokenString = NULL;
        return getLexeme();
    }
    return lex;
}