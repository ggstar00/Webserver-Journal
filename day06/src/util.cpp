#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void errif(bool err,const char* msg){
    if(err){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

