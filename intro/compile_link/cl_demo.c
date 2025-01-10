#include <stdio.h>

const int c1 = 10;

int g1;

int g2 = 100;

// gcc -c cl_demo.c
// gcc -g -o sd cl_demo.c

// elf header: readelf -h <object / executable>
// elf sections: readelf -S <obj/exec>
// elf section details:  readelf -x sec <obj/executable>
int main(){
    printf("stupid stuff %d\n", g2+g1+c1);
}