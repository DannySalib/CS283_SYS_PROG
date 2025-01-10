#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "math_stuff.h"

// Note to build statically you can run make static
// The default to dynamically link the c runtime library is just make

bool isodd(int x){
    if ((x % 2) != 0)
        return true;
    else
        return false;

    //yes, i know a better way to do this is to just code
    // return ((x % 2) == 0) but this will make the 
    //demo clearer
}

int main(){
    int a, b, c;
    bool odd;

    a = 5;
    b = 20;
    c = my_add(a,b);

    printf("The value of %d + %d is %d\n", a,b,c);

    odd = isodd(c);
    if (odd)
        printf("The result is odd\n");
    else
        printf("The result is even\n");

    exit(0);
}