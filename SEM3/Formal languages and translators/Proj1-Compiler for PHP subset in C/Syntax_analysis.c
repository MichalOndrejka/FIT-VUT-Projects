/* ************************ Syntax_analysis.c **************************** */
/* ********** Implemetácia prekladaču imperatívneho jazyka IFJ22 ********* */
/*  Predmet: Formálne jazyky a prekladače (IFJ) - FIT VUT v Brne           */
/*  Team: xondre15, xtruch01, xbrnak01, xhrach06                           */
/*  Vytvoril: xhrach06, xtruch01, xbrnak01                                 */
/* *********************************************************************** */

#include "Syntax_analysis.h"

void clear_all(){
    free(Token);
    Token=NULL;
    free(fun);
    fun=NULL;
    bst_dispose(&tree);
    tree=NULL;
    assign_function=NULL;
}

void clear_token(){
	Token->type = T_VOID;
    Token->param_is_q = false;
}
//@type_copy copy type of variable
void type_copy(Lexeme lex){
    if(strcmp("int", lex.string)==0 || strcmp("?int", lex.string)==0) Token->type=T_INT;
    if(strcmp("string", lex.string)==0 || strcmp("?string", lex.string)==0) Token->type=T_STRING;
    if(strcmp("float", lex.string)==0 || strcmp("?float", lex.string)==0) Token->type= T_FLOAT;
     if(strcmp(lex.string,"?float")==0 ||strcmp("?string", lex.string)==0 || strcmp("?int", lex.string)==0) Token->param_is_q = 1;

}
//@type_copy_add returns type of variable
type type_copy_add(Lexeme lex){
    if(strcmp("int", lex.string)==0 || strcmp("?int", lex.string)==0) return T_INT;
    else if(strcmp("string", lex.string)==0 || strcmp("?string", lex.string)==0) return T_STRING;
    else if(strcmp("float", lex.string)==0 || strcmp("?float", lex.string)==0) return T_FLOAT;
    else return T_VOID;
    return T_INT;
}

void rule_prologue(){
    Lexeme lex;
    lex = getLexeme();
    if(lex.kind != PROLOGUE){ 
        syntaxError(lex);
        return;
    }

    lex = getLexeme();
    if(lex.kind != ID || strcmp(lex.string, "declare") != 0){
        syntaxError(lex);
        return;
    }

    lex = getLexeme();
    if(lex.kind != LBRACKET){ 
        syntaxError(lex);
        return;
    }

    lex = getLexeme();
    if(lex.kind != ID || strcmp(lex.string, "strict_types") != 0){
        syntaxError(lex);
        return;
    }
    
    lex = getLexeme();
    if(lex.kind != ASSIGNMENT){
        syntaxError(lex);
        return; 
    }
    
    lex = getLexeme();
    if(lex.kind != INT && lex.value != 1){
        syntaxError(lex);
        return;
    }
    
    lex = getLexeme();
    if(lex.kind != RBRACKET){
        syntaxError(lex);
        return;
    }
    
    lex = getLexeme();
    if(lex.kind != SEMICOLON){
        syntaxError(lex);
        return;
    }
}

void rule_p_body(){
    
    rule_p_prikaz();
    rule_p_body();
}

  void rule_cond_body(){
        rule_p_prikaz();
        if(exit_condition){
            exit_condition = false;
            return;
        }
        rule_cond_body();     
    }



void rule_p_prikaz(){
    exit_without_return = false; 
    
    lex = getLexeme();
    
    if(lex.string!=NULL && strcmp(lex.string, "return") == 0 && in_function == false){
        syntaxError(lex);
        return;
    }
    if (lex.kind == RCURLY && in_condition > 0){
        if (in_while > 0){
            printf("JUMP INCONDITION%d%d\n", while_count , in_condition);
        }
        exit_condition = true;
        return;
    }
    if(lex.kind == RCURLY && in_function == true){
        exit_function = true;
        exit_without_return = true;
        return;
    }
    if (lex.string != NULL){
        if (strcmp(lex.string, "return") == 0 && in_function == true){
            exit_function = true;
            exit_return = true;
            return;
        }
    }


    //p_prikaz -> func_call

  
    switch(lex.kind)
    {
        case KEYWORD:
            //p_prikaz -> function ID ( params ) : func_types { func_body }
            if(strcmp(lex.string, "function") == 0){
                if(in_function || in_condition > 0){
                    syntaxError(lex);
                }
                
                lex = getLexeme();
                if(lex.kind != ID){
                    syntaxError(lex);
                   return; 
                }
                if(bst_search(tree, lex.string) != NULL){
                    clear_all();
                    exit(3);
                    return;
                }
                fun = realloc(fun, strlen(lex.string));
                if (fun == NULL) {
                    clear_all();
                    exit(99);
                }
                strcpy(fun, lex.string);
                bst_insert(&tree, fun);

                printf("JUMP IFDEF%s\n", lex.string);
                printf("LABEL %s\n", lex.string);
                printf("PUSHFRAME\n");

                lex = getLexeme();
                if(lex.kind != LBRACKET){
                    clear_all();
                    exit(2);
                    return;
                }
                
                lex = getLexeme();
                if(lex.kind == RBRACKET){
                    NODE *current = bst_search(tree, fun);
                    current->param_count = 0;
                } else{
                    rule_params(lex);
                }
                
                lex = getLexeme();
                if(lex.kind != COLON){
                    clear_all();
                    exit(2);
                    return;
                }
                rule_func_typesq();
                
                lex = getLexeme();
                if(lex.kind != LCURLY){
                    clear_all();
                    exit(2);
                }

                rule_func_body();

                if(!exit_without_return){
                    lex = getLexeme();
                }
                
                if(lex.kind != RCURLY){
                    clear_all();
                    exit(2);
                }

                fun = realloc(fun,sizeof(char));
                if (fun == NULL) {
                    clear_all();
                    exit(99);
                }
                strcpy(fun, "1");
                param_def_count=0;
            }
            //p_prikaz -> if ( expression ) { p_prikaz } else { p_prikaz } 
            else if(strcmp(lex.string, "if") == 0){
                in_condition += 1;
                label_count += 1;
                lex = getLexeme();
                if(lex.kind != LBRACKET){
                    clear_all();
                    exit(2);
                }
                startExpression();
                while (lex.kind!=LCURLY){
                    rule_expression(lex, fun, tree);
                   
                    lex = getLexeme();
                }
                if(endExpression(fun, tree) == INVALID){clear_all(); exit(4); return;}
                printf("JUMPIFNEQ ELSE%d%d GF@var1 bool@true\n", label_count, in_condition);
                rule_cond_body();
                printf("JUMP ENDIF%d%d\n",label_count, in_condition);
                lex = getLexeme();
                if(lex.kind != KEYWORD || strcmp(lex.string, "else") != 0){
                    clear_all();
                    exit(2);
                }
                printf("LABEL ELSE%d%d\n", label_count,in_condition);
                lex = getLexeme();
                if(lex.kind != LCURLY){
                    clear_all();
                    exit(2);
                }
                
                rule_cond_body();
                if(lex.kind != RCURLY){
                    clear_all();
                    exit(2);
                }
                printf("LABEL ENDIF%d%d\n", label_count,in_condition);
                in_condition -= 1;
            }
            //p_prikaz -> while ( expression ) { p_prikaz }
            else if(strcmp(lex.string, "while") == 0){
                in_condition += 1;
                in_while += 1;
                while_count += 1;
                lex = getLexeme();
                if(lex.kind != LBRACKET){
                    clear_all();
                    exit(2);
                    return;
                }
                printf("LABEL INCONDITION%d%d\n",  while_count, in_condition);
                startExpression();
                while (lex.kind!=LCURLY){
                    rule_expression(lex, fun, tree);
                    lex = getLexeme();
                }
                
    
                if(endExpression(fun, tree) == INVALID){exit(4); return;}
                printf("JUMPIFNEQ ENDWHILE%d%d GF@var1 bool@true\n",  while_count, in_condition);
                rule_cond_body();
                if(lex.kind != RCURLY){
                    clear_all();
                    exit(2);
                    return;
                }
                printf("LABEL ENDWHILE%d%d\n",  while_count, in_condition);
                in_condition -= 1;
            }
            else{
                clear_all();
                exit(2);
                return;
            }
            break;
        //p_prikaz -> func_call ;
        case ID: ;
            //if(bst_search(tree, lex.string) == NULL){
            //exit(2);
            //}
            char *return_to = NULL;
            return_to = realloc(return_to,strlen(fun));
            if (return_to == NULL) {
                clear_all();
                exit(99);
            }
            strcpy(return_to, fun);
            fun = realloc(fun, strlen(lex.string));
            if (fun == NULL) {
                clear_all();
                exit(99);
            }   
            strcpy(fun, lex.string); 
            rule_func_call(return_to); 
            fun = realloc(fun, strlen(return_to));  
            if (fun == NULL) {
                clear_all();
                exit(99);
            }
            strcpy(fun, return_to);
            free(return_to);
            param_in_count=0;
            break;
        case VAR:
            rule_assignement(lex);
            clear_token();
            break;
        case LEOF:
            clear_all();
            exit(0);
            
        case END:
            clear_all();
            exit(0);
            
        default:
            clear_all();
            exit(2);
            return;
    }
}

//func_typesq -> types
//func_typesq -> void
void rule_func_typesq(){
    
     lex = getLexeme(); 
    if(lex.kind != KEYWORD || (strcmp(lex.string, "int") != 0 && strcmp(lex.string, "float") != 0 && strcmp(lex.string, "string") != 0 && strcmp(lex.string, "void") != 0 && strcmp(lex.string, "?int") != 0 && strcmp(lex.string, "?float") != 0 && strcmp(lex.string, "?string") != 0)){
        clear_all();
        exit(2);
    }
        if(lex.string[0]=='?')  bst_search(tree, fun)-> return_type_is_q=1;
        token *add_type =(token *) malloc(sizeof(token));
        if (add_type == NULL) {
            clear_all();
            exit(99);
        }

        add_type->type = type_copy_add(lex);
        bst_addtype(fun, &tree , add_type->type);
        free(add_type);
}

//p_end -> EOF
//p_end -> ?>
void rule_p_end(){
    
     lex = getLexeme(); 
    if(lex.kind != END || lex.kind != LEOF){
        clear_all();
        exit(2);
    }
} 

void rule_assignement(Lexeme VariableLex){
    lex = getLexeme();
    if(lex.kind != ASSIGNMENT){
        clear_all();
        exit(2);
    }
    lex = getLexeme();
    if(lex.kind == ID){
        if(bst_search(tree, lex.string) == NULL && strcmp(lex.string, "reads") != 0 && strcmp(lex.string, "readi") != 0 && strcmp(lex.string, "readf") != 0) {   /////$abc = func1()
            clear_all();
            exit(2);
        }
        char *return_to = NULL;
        return_to = realloc(return_to,strlen(fun));
        if (return_to == NULL) {
            clear_all();
            exit(99);
        }
        strcpy(return_to, fun);        
        strcpy(fun, lex.string);;
        rule_func_call(return_to); 
        NODE *current = bst_search(tree, return_to);
        if(!bst_search_local(current->local, VariableLex.string)){
        bst_insert_local(&current->local, VariableLex.string, Token);   
        printf("DEFVAR LF@%s\n", VariableLex.string);
        printf("POPS LF@%s\n", VariableLex.string);
        } else{
            token *to_update = bst_search_local(current->local, VariableLex.string);
            to_update->type = Token->type;
            printf("POPS LF@%s\n", VariableLex.string);
        }
        fun = realloc(fun, strlen(return_to));
        if (fun == NULL) {
            clear_all();
            exit(99);
        }
        strcpy(fun, return_to);

        free(return_to);

    }
    else {
        startExpression();
        while(lex.kind!=SEMICOLON && lex.kind!=LEOF){
            rule_expression(lex, fun, tree);
            
            lex = getLexeme();
        }
        ExpressionType typeExp = endExpression(fun, tree);
        if(typeExp == INVALID) exit(7);
        
        NODE *current = bst_search(tree, fun);

        if (!bst_search_local(current->local, VariableLex.string)){
            switch(typeExp){
                    case PINT: Token -> type = T_INT; break;
                    case PSTRING: Token -> type = T_STRING;  break;
                    case PFLOAT: Token -> type = T_FLOAT; break;
                    default:
                        printf("NEPODPORUJEME\n");
                        exit(8);
            }
            bst_insert_local(&(current->local), VariableLex.string, Token);
            printf("DEFVAR LF@%s\n", VariableLex.string);
            printf("POPS LF@%s\n", VariableLex.string);
             
            } 
            else{
                token *to_update = bst_search_local(current->local, VariableLex.string);
                switch(typeExp){
                    case PINT: to_update -> type = T_INT; break;
                    case PSTRING: to_update -> type = T_STRING; break;
                    case PFLOAT: to_update -> type = T_FLOAT; break;
                    default:
                        printf("NEPODPORUJEME\n");
                        exit(8);
                }
                printf("POPS LF@%s\n", VariableLex.string);
            }
    }
}

 //types -> int
 //types -> float
 //types -> string
void rule_types(){
    
     lex = getLexeme(); 
    if(lex.kind != KEYWORD && (strcmp(lex.string, "int") != 0 || strcmp(lex.string, "float") != 0 || strcmp(lex.string, "string") != 0 || strcmp(lex.string, "?int") != 0 || strcmp(lex.string, "?float") != 0 || strcmp(lex.string, "?string") != 0)){
        if(lex.string[0]=='?')  {exit(2); return;}
        exit(2);
        return;
    }
}

//params -> var_types VAR params_next
//params_next -> , var_types VAR params_next 
//params_next -> ε
void rule_params(Lexeme lex){

    param_def_count++; //treba vynulovat pred volanim rule_params
    if(lex.kind != KEYWORD && (strcmp(lex.string, "int") != 0 || strcmp(lex.string, "float") != 0 || strcmp(lex.string, "string") != 0 || strcmp(lex.string, "?int") != 0 || strcmp(lex.string, "?float") != 0 || strcmp(lex.string, "?string") != 0)){
        clear_all();
        exit(2);
    } 
                // TODO 
    
    NODE *current = bst_search(tree,fun);
    current->param_count++;
    type_copy(lex);

    current->params[param_def_count -1] = type_copy_add(lex);
    
    lex = getLexeme();

    printf("DEFVAR LF@%s\n", lex.string);
    printf("MOVE LF@%s LF@%%%d\n", lex.string, param_def_count);

    if(lex.kind != VAR){
        clear_all();
        exit(2);
    }
   
    if (bst_search_local(current->local, lex.string) != NULL){
        clear_all();
        exit(8);
    }
    
    current = bst_search(tree,fun);
    bst_insert_local( &(current->local), lex.string,  Token);
    

   
    current->params_names[param_def_count - 1]= malloc(strlen(lex.string));
    if (current->params_names[param_def_count - 1] == NULL) {
        clear_all();
        exit(99);
    }
    strcpy(current->params_names[param_def_count - 1], lex.string);
    
    lex = getLexeme();
    if(lex.kind!=RBRACKET){
        if(lex.kind != COMMA){
            clear_all();
            exit(2);
        }
        clear_token();
        
        lex = getLexeme(); 
        rule_params(lex);
    }
    else{
        return;
    }
}

//params_in -> expression DEFVAR LF
//params_in -> VAR
//params_in_next -> , params_in params_in_next
//params_in_next -> ε
void rule_params_in(Lexeme lex, char *return_to){
    printf("CREATEFRAME\n");
    while(lex.kind != RBRACKET){
        if(lex.kind == VAR){
            param_in_count++;
            NODE *current = bst_search(tree,fun);
            NODE *current_return_to = bst_search(tree, return_to);
            token *current_param = bst_search_local(current_return_to->local, lex.string);
            if ( current_param == NULL )
            {
                clear_all();
                exit(4);
            }

            if(current->params[param_in_count-1] != current_param->type){
                clear_all();
                exit(4);
            }
            printf("DEFVAR TF@%%%d\n", param_in_count);
            printf("MOVE TF@%%%d LF@%s\n", param_in_count, lex.string);
        }

        else if (lex.kind == INT){
            param_in_count++;
            NODE *current = bst_search(tree, fun);
            if(current->params[param_in_count - 1] != T_INT){
                clear_all();
                exit(4);
            }
            printf("DEFVAR TF@%%%d\n", param_in_count);
            printf("MOVE TF@%%%d int@%d\n", param_in_count, lex.value);
        }
            
        else if (lex.kind == STRING){
            param_in_count++;
            NODE *current = bst_search(tree,fun);
            if(current->params[param_in_count-1] != T_STRING){
                clear_all();
                exit(4);
            }
            lex.string++;
            lex.string[strlen(lex.string)-1] = '\0';
            printf("DEFVAR TF@%%%d\n", param_in_count);
            printf("MOVE TF@%%%d string@%s\n", param_in_count, lex.string);
        }
        else if (lex.kind == FLOATA || lex.kind == FLOATB){
            param_in_count++;
            NODE *current = bst_search(tree,fun);
            if(current->params[param_in_count-1] != T_FLOAT){
                clear_all();
                exit(4);
            }
            printf("DEFVAR TF@%%%d\n", param_in_count);
            printf("MOVE TF@%%%d float@%a\n", param_in_count, lex.fvalue);
            }
        else if(lex.kind == COMMA){
            //pass
        }
        else if(lex.kind == KEYWORD && strcmp(lex.string, "\"null\"")){
            param_in_count++;
            NODE *current = bst_search(tree, fun);
            char *current_parameter;
            current_parameter = (char *) malloc(strlen(current_parameter)); ///// current_parameter nemoze byt 
            if (current_parameter == NULL) {
                clear_all();
                exit(99);
            }
            strcpy(current_parameter, current->params_names[param_in_count-1]);
            if(bst_search_local(current->local, current_parameter)->param_is_q != true) exit(4);
            printf("DEFVAR TF@%%%d\n", param_in_count);
            printf("MOVE TF@%%%d nil@nil\n", param_in_count);
            free(current_parameter);

        }
        else{
            if(lex.kind!=STRING && lex.kind!=INT && lex.kind != FLOATA && lex.kind!= FLOATB){ 
                clear_all();
                exit(2);
            }
        }
        
        lex = getLexeme();
    }
    
    // TODO
    // NODE *current = bst_search(tree,fun);                

    // if(current->param_count != param_in_count){
    //     exit(4);     
    // }

    return;
}

//ret_value -> expression
//ret_value -> ε
//ret_value -> VAR
void rule_ret_value(){

    if(exit_without_return && bst_search(tree, fun)->return_type != T_VOID) {clear_all();exit(4); return;}
    if(exit_without_return) return;
    lex = getLexeme(); 
    if(lex.kind != SEMICOLON && bst_search(tree, fun)->return_type == T_VOID) {
        clear_all();
        exit(4); 
        return;
    }
   
    if(bst_search(tree, fun)->return_type != T_VOID){     
        startExpression();
        while(lex.kind!=SEMICOLON){
            if(lex.kind == KEYWORD && strcmp(lex.string, "null")==0){
                if(bst_search(tree, fun)->return_type_is_q==false) {
                    clear_all();
                    exit(4);
                }
                else break;
            }
            rule_expression(lex, fun, tree);
            lex = getLexeme();
        }
        //if(endExpression(fun, tree) == INVALID){exit(4);
        ExpressionType typeExp = endExpression(fun, tree);
        if(typeExp == INVALID) {
            clear_all(); 
            exit(7);
        }
        switch(typeExp){
            case PINT: Token -> type = T_INT; break;
            case PSTRING: Token -> type = T_STRING;  break;
            case PFLOAT: Token -> type = T_FLOAT; break;
            default:
                printf("NEPODPORUJEME\n");
                clear_all();
                exit(8);
            }

                    
    }
}

// CODE GENERATE FUNCTIONS
void writeGenerate(Lexeme lex, char *return_to){
    while(lex.kind != RBRACKET){ 
        if(lex.kind == STRING){
            lex.string++;
            lex.string[strlen(lex.string)-1] = '\0';
            printf("WRITE string@");

            for (int i = 0; i < (int)(strlen(lex.string)); i++){
                if (lex.string[i] == ' '){
                    printf("\\032");
                }
                else if (lex.string[i] == '#'){
                    printf("\\035");
                }
                else if (lex.string[i] == '\\'){
                    if(lex.string[i+1] != '\0'){
                        if(lex.string[i+1] == 'n'){
                            printf("\\010");
                            i++;
                        } 
                        else if(lex.string[i+1] == 't'){
                            printf("\\009");
                            i++;
                        } 
                        else{
                            printf("\\092");
                        }
                        }
                    }
                else{
                    printf("%c",lex.string[i]);
                }
            }
            printf("\n");
        } else if(lex.kind == VAR){
            NODE *current = bst_search(tree, return_to);
            token *findlocal = bst_search_local(current->local,lex.string);

            if(findlocal == NULL){
                clear_all();
                exit(5);
            }

            printf("WRITE LF@%s\n",lex.string);
        } else if(lex.kind == INT){
            printf("WRITE int@%d\n",lex.value);
        }
        else if(lex.kind == FLOATA){
            printf("WRITE float@%a\n",lex.fvalue);
        }
         else if(lex.kind == FLOATB){
            printf("WRITE float@%a\n",lex.fvalue);
        } else{
            clear_all();
            exit(2);
        }
        lex = getLexeme();
        if (lex.kind != COMMA && lex.kind != RBRACKET){
            clear_all();

            exit(2);
        }
    
       if(lex.kind != RBRACKET){
        lex = getLexeme();   
       }
    }
        

        
}

void rule_func_call(char *return_to){  
    lex = getLexeme();
    if(lex.kind != LBRACKET){
        clear_all();
        exit(2);
        return;
    }

    lex = getLexeme();  

    if(strcmp(fun,"write") == 0){
        writeGenerate(lex, return_to);
        lex = getLexeme();
        if(lex.kind != SEMICOLON){
            clear_all();
            exit(2);
        }
        return;
    } 
    else if(strcmp(fun,"reads") == 0){
        printf("READ GF@var1 string\n");
        printf("PUSHS GF@var1\n");
        lex = getLexeme();
        if(lex.kind != SEMICOLON){
            clear_all();
            exit(2);
        }
        return;
    }
    else if(strcmp(fun,"readi") == 0){
        printf("READ GF@var1 int\n");
        printf("PUSHS GF@var1\n");
        lex = getLexeme();
        if(lex.kind != SEMICOLON){
            clear_all();
            exit(2);
        }
        return;
    }
    else if(strcmp(fun,"readf") == 0){
        printf("READ GF@var1 string\n");
        printf("PUSHS GF@var1\n");
        lex = getLexeme();
        if(lex.kind != SEMICOLON){
            clear_all();
            exit(2);
        }
        return;
    }
    NODE *current = bst_search(tree, fun);
    if(current == NULL){clear_all(); exit(3);}

    rule_params_in(lex, return_to);

    lex = getLexeme();

    if(lex.kind != SEMICOLON){
        clear_all();
        exit(2);
    }
    printf("CALL %s\n", fun);
}

void rule_func_body(){
        in_function=true; 
        rule_p_prikaz();
        
        if (exit_function == true){
            in_function = false;
            exit_function = false;
            /*if(!exit_without_return)*/ rule_ret_value();
              printf("POPFRAME\n");
            printf("RETURN\n");
            printf("LABEL IFDEF%s\n", fun);
            return;
        }
        rule_func_body();
}

void p_start(){
    rule_prologue();
    rule_p_body();
    rule_p_end();
}

int main(){
    Token = (token *) malloc(sizeof(token));
    if (Token == NULL) {
        clear_all();
        exit(99);
    }
    fun = malloc(sizeof(char));
    if (fun == NULL) {
        clear_all();
        exit(99);
    }
    fun[0] = '1';
    bst_insert(&tree, fun);
    NODE *tree = NULL;
    bst_init(&tree);
    printf(".IFJcode22\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR GF@var1\n");
    printf("DEFVAR GF@var2\n");
    printf("DEFVAR GF@var3\n");
    printf("DEFVAR GF@var4\n");
    p_start();
    return 0;
}
