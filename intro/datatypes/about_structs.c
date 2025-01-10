//OBJECTIVE
//
// 1. What are structs
// 2. Using them as types
// 3. They really dont exist, just very helpful to avoid
//    complex pointer addition


#include <stdio.h>
#include <stdint.h>


//This is a basic struct defintion in C, to create an
//instance of this struct you have to define it as 
//"struct demo1"
struct demo1{
    int8_t  f1;
    int16_t f2;
    int32_t f3;
    int64_t f4;
};

//This is more common struct defintion in C, in this version
//the struct demo2 is defined as a type.  In this version you
//could either define an instance of it using "struct demo2" or
//"demo2_t".  Notice the "_t" at the end, this is the common 
//convention used in C to express something as a user defined 
//type.
typedef struct demo2{
    int8_t  f1;
    int16_t f2;
    int32_t f3;
    int64_t f4;
} demo2_t;

//we will talk about this in the packed demo, this is a compiler
//directive telling the compiler to lay out the fields direclty
//as specified and do not add any padding.  This can be useful
//for certain low level needs such as network programming. 
typedef struct demo3  {
    int8_t  f1;
    int16_t f2;
    int32_t f3;
    int64_t f4;
}__attribute__((packed)) demo3_t;

//prototypes so that the compiler does not complain
void basic_struct_demo(void);
void type_struct_demo(void);
void pointer_struct_demo_strange(void);
void pointer_struct_demo(void);
void packed_pointer_demo(void);

int main(){
    basic_struct_demo();
    type_struct_demo();
    pointer_struct_demo_strange();
    pointer_struct_demo();
    packed_pointer_demo();
}

void basic_struct_demo(void){
    struct demo1 d1;
    struct demo1 *pd1;

    //with structs you just use "." notation to access fields;
    d1.f1 = 1;
    d1.f2 = 2;
    d1.f3 = 3;
    d1.f4 = 4;

    printf("d1: f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        d1.f1, d1.f2, d1.f3, d1.f4);

    pd1 = &d1;

    //In C we dereference a pointer using the * operator, we
    //can dereference the struct pointer and then use dot
    //notation
    printf("pd1 (ptr notation): f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        (*pd1).f1, (*pd1).f2, (*pd1).f3, (*pd1).f4);

    //In C the -> operator is syntatic sugar for the above where
    //a pointer is dereferenced and then accessed by a dot.  This
    //is the most common and idomatically way to deal with structure
    //poiters
    printf("pd1 (arrow notation): f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        pd1->f1, pd1->f2, pd1->f3, pd1->f4);

    printf("\n");
}

void type_struct_demo(void){
    demo2_t d2;
    demo2_t *pd2;

    //same demo as basic_struct_demo but this type just
    //using a type definition so that we dont have to use
    //the struct keyword
    d2.f1 = 1;
    d2.f2 = 2;
    d2.f3 = 3;
    d2.f4 = 4;

    printf("d2: f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        d2.f1, d2.f2, d2.f3, d2.f4);

    pd2 = &d2;

    printf("pd2 (ptr notation): f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        (*pd2).f1, (*pd2).f2, (*pd2).f3, (*pd2).f4);


    printf("pd2 (arrow notation): f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        pd2->f1, pd2->f2, pd2->f3, pd2->f4);

    printf("\n");
}

void pointer_struct_demo_strange(void){
    demo2_t d2;
    void *vd2;  //void is an anonmous type, compiler assumes
                //you know what you are doing, be careful!!!

    //This time we are going to debunk that structures are
    //anything special
    d2.f1 = 1;
    d2.f2 = 2;
    d2.f3 = 3;
    d2.f4 = 4;

    vd2 = &d2;

    //because the structure is flagged as packed the bytes will
    //not be aligned.  Note that this causes some more inefficiency
    //in many cases becuase the compiler needs to generate code
    //to stick to even address boundries then mask and shift certain
    //values
    int8_t  *f1 = vd2;
    int16_t *f2 = vd2 + 1;
    int32_t *f3 = vd2 + 3;
    int64_t *f4 = vd2 + 7;

    printf("vd2_strange (pointer math): f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        *f1, *f2, *f3, *f4);

    printf("\n");
}

void pointer_struct_demo(void){
    demo2_t d2;
    void *vd2;  //void is an anonmous type, compiler assumes
                //you know what you are doing, be careful!!!

    //This time we are going to debunk that structures are
    //anything special
    d2.f1 = 1;
    d2.f2 = 2;
    d2.f3 = 3;
    d2.f4 = 4;

    vd2 = &d2;

    //Most machines can only read on byte boundries so the
    //compiler will pad things that end on non-even addresses
    int8_t  *f1 = vd2;
    int16_t *f2 = vd2 + 2;  //*** WHY 2? *****/
    int32_t *f3 = vd2 + 4;
    int64_t *f4 = vd2 + 8;

    printf("vd2 (pointer math): f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        *f1, *f2, *f3, *f4);

    printf("\n");
}

void packed_pointer_demo(void){
    demo3_t d3;
    void *vd3;  //void is an anonmous type, compiler assumes
                //you know what you are doing, be careful!!!

    //This time we are going to debunk that structures are
    //anything special
    d3.f1 = 1;
    d3.f2 = 2;
    d3.f3 = 3;
    d3.f4 = 4;

    vd3 = &d3;

    int8_t  *f1 = vd3;
    int16_t *f2 = vd3 + 1;
    int32_t *f3 = vd3 + 3;
    int64_t *f4 = vd3 + 7;

    printf("vd3_packed (pointer math): f1:%d, f2:%d, f3:%d, f4:%ld\n", 
        *f1, *f2, *f3, *f4);

    printf("\n");
}
