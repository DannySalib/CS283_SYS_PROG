#include <stdio.h>
#include <stdint.h>

long fn(long d_a1, long d_a2){
    long d_v1 = 3;
    long d_v2 = 4;
    long d_v3 = 5;

    printf("Address d_a1: %p\n", &d_a1);
    printf("Address d_a2: %p\n", &d_a2);
    printf("Address d_v1: %p\n", &d_v1);
    printf("Address d_v2: %p\n", &d_v2);
    printf("Address d_v3: %p\n", &d_v3);
    printf("\n");

    return d_a1 + d_a2 + d_v1 + d_v2 + d_v3;
}

long messedUp(){
    long d_mv1 = 1;
    long d_mv2 = 2;

    printf("BEFORE d_mv1 [%p]:%ld\n", &d_mv1, d_mv1);
    printf("BEFORE d_mv2 [%p]:%ld\n", &d_mv2, d_mv2);
    printf("\n");

    //Note we are subtracting 1, which should be considered one unit
    //of the current type, which is long.  So -1 on a 64 bit value 
    //will subtract 8 bytes
    *((&d_mv2 - 1)) = 3;

    printf("AFTER1 d_mv1 [%p]:%ld\n", &d_mv1, d_mv1);
    printf("AFTER1 d_mv2 [%p]:%ld\n", &d_mv2, d_mv2);
    printf("\n");

    //to work with true byte arithmatic we need to treat the pointer
    //as a pointer to bytes versus a pointer to longs
    uint8_t *byte_ptr = (uint8_t *)&d_mv2;
    *(byte_ptr - 8) = 4;

    printf("AFTER2 d_mv1 [%p]:%ld\n", &d_mv1, d_mv1);
    printf("AFTER2 d_mv2 [%p]:%ld\n", &d_mv2, d_mv2);
    printf("\n\n");

    //in C void * also work on byte boundries.  
    void *v_ptr = &d_mv2;
    *(long *)(v_ptr - 8) = 5; //the compiler will compain without the cast

    printf("AFTER3 d_mv1 [%p]:%ld\n", &d_mv1, d_mv1);
    printf("AFTER3 d_mv2 [%p]:%ld\n", &d_mv2, d_mv2);
    printf("\n\n");


    return d_mv1+d_mv2;
}

int main(){
    fn(1,2);
    messedUp();

    printf("Note pointers are shown as 48-bit values\n");
    printf("This is because of Intel/AMD and ARM standards\n");
    printf("So they are really large positive numbers\n");
    printf("Run this program multiple times\n"); 
    printf("Why do the values change from run to run?\n");

    printf("The address of fun() = %p\n", fn);
}