//compile with: gcc -nostartfiles -o startv2 2-startv1.c
long syscall_error(long err) {
    // Negate the error value to get positive errno
    return -err;
}

//From https://github.com/bminor/glibc/blob/master/sysdeps/unix/sysv/linux/aarch64/syscall.S
long syscall(long number, long a1, long a2, long a3, 
             long a4, long a5, long a6) {
    register long syscall_num __asm__("x8") = number;
    register long arg0 __asm__("x0") = a1;
    register long arg1 __asm__("x1") = a2;
    register long arg2 __asm__("x2") = a3;
    register long arg3 __asm__("x3") = a4;
    register long arg4 __asm__("x4") = a5;
    register long arg5 __asm__("x5") = a6;

    __asm__ volatile (
        "svc 0x0\n\t"
        "cmn x0, #4095\n\t"
        "b.cs 1f\n\t"
        "ret\n"
        "1:\n\t"
        "b syscall_error"
        : "+r" (arg0)
        : "r" (syscall_num), "r" (arg1), "r" (arg2), 
          "r" (arg3), "r" (arg4), "r" (arg5)
        : "memory"
    );

    return arg0;
}

void _start(){
    int i;
    i = 20;
    syscall(93,i,0,0,0,0,0);
}