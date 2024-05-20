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
    NUM, ADD, SUB, MUL, DIV, OP_PAR, CL_PAR,
}TokenType;

typedef struct{
    char* content;
    TokenType type;
}Token;

typedef struct{
    Token tokens[MAX_TOKS];
    size_t size;
}TokenList;

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

// Takes the expression and returns an array of tokens
static TokenList tokenizer(const char * expression){
   TokenList list; 
   int i = 0;
   char * t = strtok(expression, DELIM);
   list.size = 0;
   while(t != NULL){
    list.tokens[i].content = t;
    list.tokens[i].type = get_token_type(t);
    t = strtok(NULL, DELIM);
    list.size++;
    i++;
   }
   return list;
}

#include <stdio.h>
double evaluate(const char * expression){
    if(!validate_string(expression)){
        return NAN;
    } 
    TokenList list = tokenizer(expression);
    for(int i = 0; i < list.size; i++){
        printf("Str: %s, tok: %d\n", list.tokens[i].content, list.tokens[i].type);
    }
    return 0.0;
}



#endif // DEFINE_CHEVAL
#endif
