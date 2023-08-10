#include "util.h"
#include <stdio.h>  //包含perror函数
#include <stdlib.h> //包含exit函数，EXIT_FAILURE常量，EXIT_SUCCESS常量

void errif(bool err,const char* msg){
    if(err){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}