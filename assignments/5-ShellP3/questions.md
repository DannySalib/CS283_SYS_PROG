1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

To ensure that all child processes are completed before the shell continues, I used the waitpid() fucntion. waitpid() ensures that our shell does not continue unless the child processes terminates, stops, or resumes. After proper execution, the output can now be piped into the next command. If waitpid() is not used, the process may never finish. This leads to zombie processes which can eventually cause a error in memory. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to use close() on unused pipe ends after calling dup2() to ensure that a child process terminates after the program moves along the pipeline. If we do not use close(), we depend on a few things so that our program does not crash:
- close() frees up file descriptors. Unless we have unlimited file descriptors, the shell will eventually crash.
- close() allows for the next command to read the write of the previous process. 

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command changes the current directory of your shell. It must be built in since creating a child process to execute cd can't change the directory of the parent shell. Therefore cd in nature is not a child process. A challenge that would arise from trying to use execvp() would be making the new current terminal the child process indefinitely. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To allow an arbitrary number of piped commands, memory should be allocated based on the number of pipe symbols multiplied by the max size of each command. After pipeline execution, memory is freed and the shell continues. A tardeoff to consider is that sometimes, memory won't be allocated returning NULL. This would most likely mean the user's input cannot be processed or can only be processed partially. This overall is a poorer user experience. 
