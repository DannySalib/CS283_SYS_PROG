//This code is intended to show the different ways memory 
//can be created and managed in c

//compile with: gcc -g -o mem-static mem-static.c

//see the details: readelf -s ./mem-static | grep "\bd_"
//    now lets cross reference the sections for .data, .bss, and .rodata
//see the sections readelf -S -W ./mem-static
//also these commands
//     objdump -x ./mem-static | grep "\bd_" to see what sections these variables are in
// discussion topics:
//    what is .bss vs .data
//    how is readonly applied
//    also look at .text to see how its flagged as executable
#include <stdio.h>
#include <stdlib.h>     //where we pickup malloc()/free()

int demo_fn1(int);
int demo_fn2(int);
int fn99(int);

//note all variable start with "d_" so we can easily isolate them
//in the readelf command with grep, no other reason
int d_glob_not_init;
int d_glob_zero_init = 0;
int d_glb_init = 100;

static int d_stat_not_init;
static int d_stat_zero_init = 0;
static int d_stat_init = 200;

const int d_const_init = 300;
const static int d_const_stat_init = 400;

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
    two_ints_t ptr_ints;
    int ret_val;

    ptr_ints.fld1 = 20;
    ptr_ints.fld2 = 30;

    ret_val = x + ptr_ints.fld1 + ptr_ints.fld2;

    return ret_val;
}

int main(){
    int f1 = demo_fn1(50);
    int f2 = demo_fn2(150);
    int f3 = demo_fn3(250);

    printf("fn1()=%d, fn2()=%d, and fn3()=%d\n", f1, f2, f3);
}