#include <stdio.h>

#include <stdint.h>

void sysprog_types(void);

//discussion points - there are no types, they are used by the compiler
//everyting is 1 and 0.  C is a little confusing given the size of the
//standard types has changed over the years as processor architectures
//have expanded from 16 to 32 to 64 bit widths.

int main(){
    char c;
    int i;
    long l;
    long long ll;

    //why ld?  its because of sizeof
    printf("The size of char is %ld\n", sizeof(c));
    printf("The size of int is %ld\n", sizeof(i));
    printf("The size of long is %ld\n", sizeof(l));
    printf("The size of long is %ld\n", sizeof(ll));


    //where do these values come from?
    //unitialized stack, not inited
    printf("The value of char is %c\n", c);
    printf("The value of int is %d\n", i);
    printf("The value of long is %ld\n", l);
    printf("The value of long is %lld\n", ll);

    sysprog_types();
}


//In systems programming we often want to have better control over our
//types
void sysprog_types(){
    uint8_t  b;
    uint16_t bb;
    uint32_t bbbb;
    uint64_t bbbbbbbb;

    //Most modern programming languages make it more clear the size
    //of datatypes - consider go and rust.  
    // Opportunity to talk about what signed vs unsigned is.  Really
    //they are just all bits, marking a variable as unsigned tells
    //the processor that the number is encoded in 2s complment format
    printf("\n");
    printf("The size of uint8_t is %ld\n", sizeof(b));
    printf("The size of uint16_t is %ld\n", sizeof(bb));
    printf("The size of uint32_t is %ld\n", sizeof(bbbb));
    printf("The size of uint64_t is %ld\n", sizeof(bbbbbbbb));
}
