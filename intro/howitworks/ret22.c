//compile:  gcc -o r22 ret22.c
//    run:  ./r22; echo $?
//should produce 22 in shell

#include <stdlib.h>

int main(){
    //under the covers exit() is doing some 
    //cleanup based on initializing stdlib
    //look in stdlib.h
    exit(22);
}