#include <stdlib.h>     //std library
#include <stdio.h>      //std input/output
#include <stdbool.h>    //std booleans

#define IMPLEMENT_CHEVAL
#include "../cheval.h"

int main (int argc, char **argv){
    char * input = argv[1];
    printf("%lf\n", evaluate(input));
    return 0;
}













