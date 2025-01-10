// Discussion points
//
// compile with: gcc -nostartfiles -o startv1 1-startv1.c 
//     run with: ./startv1; echo $?


#include <unistd.h>

void _exit(int);

void _start(){
    _exit(83);
}