1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

- gets() does not account for buffer boundaries 
- fgets() handles EOF in a robust manner 
- allows us to parse the string (we added a null terminator at the end of user's input)

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

malloc() dynamically adds memory during runtime which allows us to skip the consideration of how much static memory should be allocated to the buffer via a fixed array. What if we have too little/too much memory? We would get errors or we would waste memory. Both should be avoided. 

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

Let's assume we do not account for white spaces and the user wants to exit:
- 'exit' != 'exit ' or '\t exit' etc: How do we account for this
Also, white spaces are unecessary in terms of storing our commands onto an array, therefore by not accounting for it, our memory requirements may be greater and can lead to problems later down the road. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

1. `echo hello > out`
- '>' or '>>' is STDOUT where '>' overwrites STDOUT onto a file and '>>' appends STDOUT to a file
2. `sort < data.txt`
- '<' can redirect STDIN to set the input to a file destination
3.  `test.sh 2> error.txt`
- 2>, 2>>, and &> can STDERR to a file destination in different ways

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

"Pipes" do not need file destinations. Instead, the output of one command becomes the input for the next command. Also, pipes are faster since they opt for in-buffer memory to process each command unlike redirects. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

By catagorizing errors as STDERR, we can process output and error uniqely. We separate data from errors, improve debugging, and now we can ensure relaible piping among multiple commands. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

- Detect command failures using exit codes.
- Separate STDOUT and STDERR by default but allow merging (2>&1).
- Provide useful error messages and exit codes.
- Support redirection and logging to help debugging.