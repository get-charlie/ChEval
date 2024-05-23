// Calculator example made with ChEval
#include <stdlib.h>
#include <stdio.h>
#include <math.h>       

#define IMPLEMENT_CHEVAL
#include "cheval.h"

int main (int argc, char **argv){
    if(argc != 2){
        printf("Use: calc <expression>\n");
        return 1;
    }
    double res = evaluate(argv[1]);
    if(isnan(res)){
        printf("Error!\n");
        return 1;
    }
    printf("%.2lf\n", res);
    return 0;
}













