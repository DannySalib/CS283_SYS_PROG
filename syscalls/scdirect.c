#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>

int main() {
    const char *message = "Hello, portable syscall!\n";
    
    // Use syscall() for a portable system call
    long result = syscall(SYS_write, STDOUT_FILENO, message, 25);
    
    // Check for errors
    if (result == -1) {
        perror("syscall failed");
        return 1;
    }
    
    return 0;
}