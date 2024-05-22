#ifndef _CH_EVAL_
#define _CH_EVAL_

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 

// Takes an expression as a string and returns the result as a double value.
// If an error is detected NAN is returned. 
double evaluate(const char * expression);

#ifdef  IMPLEMENT_CHEVAL

#define VALID_CHARS " 0123456789.+-*/()"
#define DELIM       " "
#define MAX_TOKS    100

typedef enum{
    NUM, ADD, SUB, MUL, DIV, OP_PAR, CL_PAR, ERROR,
}TokenType;


typedef struct{
    char* content;
    TokenType type;
}Token;

typedef struct{
    Token tokens[MAX_TOKS];
    size_t size;
}TokenList;


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

int get_priority(Token tok){
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

// Test if the string contains invalid chars. true if ok false otherwise.
static bool validate_string(const char * expression){
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
static TokenList tokenizer(const char * expression){ // TODO make tokenizer not depend on white space
    TokenList list; 
    int i = 0;
    char * t = strtok(expression, DELIM);
    list.size = 0;
    while(t != NULL){
        Token new; new.content = t; new.type = get_token_type(t);
        list_push(&list, new);
        t = strtok(NULL, DELIM);
        i++;
    }
    return list;
}

static TokenList get_rpn(TokenList in){ // Shunting Yard algorithm
    TokenList out; out.size = 0;
    TokenList stack; stack.size = 0;
    for(int i = 0; i < in.size; i++){
        Token tok = in.tokens[i];
        if(tok.type == NUM){
            list_push(&out, tok);
        }else if(tok.type == OP_PAR){
            list_push(&stack, tok);
        }else if(tok.type == CL_PAR){ // TODO Error checking
            while(list_peek(stack).type != OP_PAR){
                list_push(&out, list_pop(&stack));
            }
            list_pop(&stack);
        }else{
            if(get_priority(tok) <= get_priority(list_peek(stack))){
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
        default:
            return NAN;
    }
}

static double calculate_result(TokenList rpn){
    double stack[MAX_TOKS];
    size_t size = 0;
    for(int i = 0; i < rpn.size; i++){
        Token tok = rpn.tokens[i];
        if(tok.type == NUM){
            stack[size] = strtod(tok.content , NULL); 
            size++;
        }else if(size >= 2){
            double a = stack[size - 2];
            double b = stack[size - 1];
            stack[size-2] = operation(a, b, tok);
            size--;
        }else{
            return NAN; // An error acoured
        }
    }
    if(size != 1){
        return NAN;
    }
    return stack[size-1];
}

#include <stdio.h> // Testing
static void print_list(TokenList list){
    for(int i = 0; i < list.size; i++){
        printf("%s ", list.tokens[i].content);
    }
    printf("\n");
}

double evaluate(const char * expression){
    if(!validate_string(expression)){
        return NAN;
    } 
    TokenList list = tokenizer(expression);
    // print_list(list);
    TokenList rpn = get_rpn(list);
    // print_list(rpn);
    return calculate_result(rpn);
}



#endif // DEFINE_CHEVAL
#endif
