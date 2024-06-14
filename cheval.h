#ifndef _CH_EVAL_
#define _CH_EVAL_

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 

// Takes an expression as a string and returns the result as a double value.
// If an error is detected NAN is returned. 
double cheval(const char * expression);

#ifdef  IMPLEMENT_CHEVAL

#define OPERANDS  ".0123456789"
#define OPERATORS "+-*/()"
#define VALID_CHARS OPERANDS OPERATORS " "
#define MAX_TOKS    100
#define TOK_LEN     32

typedef enum{
    NUM, ADD, SUB, MUL, DIV, OP_PAR, CL_PAR, ERROR,
}TokenType;

typedef struct{
    char content[TOK_LEN];
    TokenType type;
}Token;

typedef struct{
    Token tokens[MAX_TOKS];
    size_t size;
}TokenList;

// Helper token list/stack funcions
static void list_push(TokenList* list, Token tok){
    if(list->size < MAX_TOKS){
        list->tokens[list->size] = tok;
        list->size++;
    }
}
static Token list_pop(TokenList* list){
    Token tok; tok.type = ERROR;
    if(list->size > 0){
        tok = list->tokens[list->size - 1];
        list->size--;
    }
    return tok;
}
static Token list_peek(TokenList list){
    Token tok; tok.type = ERROR;
    if(list.size > 0){
        tok = list.tokens[list.size - 1];
    }
    return tok;
}
// Helper function to append a char to the end of a string
static void append_char(char* str, char c){
    size_t len = strlen(str);
    str[len] = c;
    str[len+1] = '\0';
}
// Helper function to get the type of a token
static TokenType get_token_type(const char * content){
   if(strcmp(content, "+") == 0){
        return ADD;
   }else if(strcmp(content, "-") == 0){
        return SUB;
   }else if(strcmp(content, "*") == 0){
        return MUL;
   }else if(strcmp(content, "/") == 0){
        return DIV;
   }else if(strcmp(content, "(") == 0){
        return OP_PAR;
   }else if(strcmp(content, ")") == 0){
        return CL_PAR;
   }else{
        return NUM;
   }
}
// Helper function to get the priority of a token
static int get_token_priority(Token tok){
    switch(tok.type){
        case NUM:
            return 0;
        case OP_PAR:
        case CL_PAR:
            return 1;
        case ADD:
        case SUB:
            return 2;
        case MUL:
        case DIV:
            return 3;
        default:
            return -1;
    }
}
// Helper function for elemental operations
static double operation(double a, double b, Token tok){
    switch(tok.type){
        case ADD:
            return a + b;
        case SUB:
            return a - b;
        case MUL:   
            return a * b;
        case DIV:
            return a / b;
        default: // When an ERROR token is found NAN is returned.
            return NAN;
    }
}
// Test if the string contains invalid chars. true if ok false otherwise.
static bool input_check(const char * expression){
    size_t len = strlen(expression);
    int i = 0;
    bool ok = true;
    while(i < len && ok){
        if(strchr(VALID_CHARS, expression[i]) == NULL){
            ok = false;
        }
        i++;
    }
    return ok;
}
// Takes the expression and returns an array of tokens
static TokenList tokenizer(const char * expression){ 
    TokenList list; list.size = 0;
    size_t len = strlen(expression);
    Token new; new.content[0] = '\0';
    for(int i = 0; i < len; i++){
        char c = expression[i];
        if(strchr(OPERANDS, c) != NULL){
            append_char(new.content, c);
        }else if(strchr(OPERATORS, c) != NULL){
            if(new.content[0] != '\0'){
                new.type = get_token_type(new.content); 
                list_push(&list, new);
                new.content[0] = '\0';
            }
            append_char(new.content, c);
            new.type = get_token_type(new.content);
            list_push(&list, new);
            new.content[0] = '\0';
        }
    }
    if(new.content[0] != '\0'){
        new.type = get_token_type(new.content);
        list_push(&list, new);
    }
    return list;
}
static TokenList get_rpn(TokenList in){ // Shunting Yard algorithm
    TokenList out; out.size = 0;
    TokenList stack; stack.size = 0;
    bool error = false;
    for(int i = 0; i < in.size && !error; i++){
        Token tok = in.tokens[i];
        if(tok.type == NUM){
            list_push(&out, tok);
        }else if(tok.type == OP_PAR){
            list_push(&stack, tok);
        }else if(tok.type == CL_PAR){
            while(list_peek(stack).type != OP_PAR && !error){
                Token aux_tok = list_pop(&stack);
                list_push(&out, aux_tok);
                if(aux_tok.type == ERROR){ // Opening parenthesis not found.
                    error = true;
                }
            }
            list_pop(&stack);
        }else{
            if(get_token_priority(tok) <= get_token_priority(list_peek(stack))){
                list_push(&out, list_pop(&stack));
            }
            list_push(&stack, tok);
        }
    }
    while(stack.size > 0){
        list_push(&out, list_pop(&stack));
    }
    return out;
}

// Takes a reverse polish notation token list and calculates its value
static double evaluate_rpn(TokenList rpn){
    double stack[MAX_TOKS];
    size_t size = 0;
    for(int i = 0; i < rpn.size; i++){
        Token tok = rpn.tokens[i];
        if(tok.type == NUM){
            stack[size] = strtod(tok.content, NULL); 
            size++;
        }else if(size == 1 && (tok.type == ADD ||tok.type == SUB)){
            if(tok.type == SUB){
                stack[size-1] *= -1;
            }
        }else if(size >= 2){
            double a = stack[size - 2];
            double b = stack[size - 1];
            double res = operation(a, b, tok);
            if(isnan(res)){
                return NAN; // An error ocoured
            }
            stack[size-2] = res;
            size--;
        }else{
            return NAN; // An error ocoured
        }
    }
    if(size != 1){
        return NAN;
    }
    return stack[size-1];
}
// API funciton
double cheval(const char * expression){
    if(!input_check(expression)){
        return NAN;
    } 
    TokenList tok_list = tokenizer(expression);
    tok_list = get_rpn(tok_list);
    return evaluate_rpn(tok_list);
}

#endif // DEFINE_CHEVAL
#endif
