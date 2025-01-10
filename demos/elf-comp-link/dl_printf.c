#include <stdio.h>

//executable: gcc -g -fno-pie -no-pie -o dl_printf dl_printf.c   
//object:     gcc -g -c -fno-pie -no-pie -o dl_printf.o dl_printf.c 
int main(){
    int i;

    i = 1;
    printf("Calling printf - time %d\n",i);
    i++;
    printf("Calling printf - time %d\n",i);
}