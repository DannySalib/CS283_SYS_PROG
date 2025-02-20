1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

Fork is used to manage processes. If you want to use execvp, its better to use fork first. After calling fork() and checking the PID, we can make sure that the command ran by execvp will run as expected and our shell will maintain an order in its scheduled tasks. 

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

If fork() fails, it most likely means that we have run out of memory or exceeded the max number of processes. If fork() returns -1, I raise an error indicating that we were not able to start a child process to run execvp. If fork() != 0 it means we should wait. Eventually we can process execvp. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

execvp() will look for your command using PATH. The executable should be in user/bin or user/local/bin and so it looks there. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

Calling wait() makes the parent process suspends execution until the child process finishes. If this doesn't happen, the processor cannot get the exit status of the child process. The childprocess isn't ended untul parent process exits. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

WEXITSTATUS() extracts the exit status (low-order 8 bits) of a terminated child process, allowing the parent to determine how the child exited, which is crucial for error handling and debugging.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

In build_cmd_buff(), we tokenize the buffer by parsing the string with the SPACE character. If at any point we come accross a single or double quote, we find the end quote (or EOF) and store all characters (white space and all) into a pointer and append it to the cmd.argv. This is good enough to be passed into execvp and run as if we were in a regular shell.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

The biggest challenge was incorperating the logic for special tokens. The first was pipe tokens. Though I have not added pipe logic, it was important to write my code accounting for this future feature. Writing a loop that can be later be refactor to icorperate pipe logic was difficult. Also tokens that were wrapped in quotes proved to be a challenge as well since it involved using memory address arithmetic, malloc onto a buffer, and placing null termintors correctly.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

Signals are a mechanism that can inform a process about any asynchronous events, such as hardware exceptions or user interruptions. Signals are different than IPC since they do not carry additional data beyond their type and are not queued if multiple signals of the same type are sent; instead, they are merged into a single instance.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

SIGKILL (Signal 9)
This signal forces a process to terminate immediately and cannot be ignored, blocked, or handled by the process.
Used when a process needs to be forcefully stopped

SIGTERM (Signal 15)
A  termination signal that allows the process to clean up resources before exiting.
Often used to stop processes gracefully, allowing them to save work or close files properly.

SIGINT (Signal 2)
Sent when the user interrupts a process
Use Case: CTRL + C to terminate program

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

When a process receives SIGSTOP, it must immediately end. Therefore, SIGSTOP cannot be caught or ignored like SIGINT. This is so that linux can use this tool to pause and continue processes easily. This is ideal for debugging scenarios. 
