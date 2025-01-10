//This code is intended to show the different ways memory 
//can be created and managed in c

//compile with: gcc -g -o mem-basics mem-basics.c

#include <stdio.h>
#include <stdlib.h>     //where we pickup malloc()/free()

int demo_fn1(int);
int demo_fn2(int);

int glob_not_init;
int glob_zero_init = 0;
int glb_init = 100;

static int stat_not_init;
static int stat_zero_init = 0;
static int stat_init = 200;

const int const_init = 300;
const static int const_stat_init = 400;

//will eb used in demo_fn3
typedef struct two_ints {
    int fld1;
    int fld2;
} two_ints_t;

int demo_fn1(int x){
    int y = 20;
    int z = 30;

    return x+y+z;
}

int demo_fn2(int x){
    int y = 20;
    static int z = 30;

    return x+y+z;
}

int demo_fn3(int x){
    two_ints_t *ptr_ints;
    int ret_val;

    ptr_ints = malloc(sizeof(two_ints_t));
    ptr_ints->fld1 = 20;
    ptr_ints->fld2 = 30;

    ret_val = x + ptr_ints->fld1 + ptr_ints->fld2;

    free(ptr_ints);

    return ret_val;
}

int main(){
    int f1 = demo_fn1(50);
    int f2 = demo_fn2(150);
    int f3 = demo_fn3(250);

    printf("fn1()=%d, fn2()=%d, and fn3()=%d\n", f1, f2, f3);
}