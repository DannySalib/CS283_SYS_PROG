#include <stdio.h>
#include <stdlib.h>     //this is where malloc() and free() are located

typedef struct two_ints {
    int fld1;
    int fld2;
} two_ints_t;

int d1(){
    two_ints_t *pti;

    pti = NULL;     //NULL used for uninitialized pointers
                    //using NULL is better than setting to 0

    printf("Size of the two_ints_t struct: %ld\n", sizeof(two_ints_t));

    pti = malloc(sizeof(two_ints_t));
    pti->fld1 = 10;
    pti->fld2 = 20;

    printf("STRUCT:  addr = %p fld1 = %d, fld2=%d\n", pti, pti->fld1, pti->fld2);

    free(pti);      //On you to do this in c
}

void d2(){
    int *a_10;
    

    a_10 = malloc(10*sizeof(int));

    for (int i = 0; i < 10; i++){
        a_10[i] = i;
    }

    printf("DARRAY:  addr = %p\n",a_10);
    for (int i = 0; i < 10; i++){
        printf("\ti[%d] = %d\n", i, a_10[i]);
    }

    printf("\n");
    free(a_10);
}

void d3(){
    int *p_10;
    

    p_10 = malloc(10*sizeof(int));

    for (int i = 0; i < 10; i++){
        *(p_10+i) = i+10;
    }

    printf("PARRAY:  addr = %p\n", p_10);
    for (int i = 0; i < 10; i++){
        printf("\ti[%d] = %d\n", i, p_10[i]);
    }

    printf("\n");
    free(p_10);
}

int main(){
    d1();
    d2();
    d3();
}