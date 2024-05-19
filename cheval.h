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

#define VALID_CHARS "0123456789.+-*/()"

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

double evaluate(const char * expression){
    if(!validate_string(expression)){
        return NAN;
    } 
    return 0.0;
}



#endif // DEFINE_CHEVAL
#endif
