## Automatic Memory Storage

compile with:
```bash
gcc -g -o mem-automatic mem-automatic.c 
```

When linux starts a process it sets up storage for local variables on the stack.  Although this limit can be dynamically changed or changed via a linux configuration, it generally is found to be 8MB.  You can check this using the `ulimit` command that shows the stack size in kilobytes:

```bash
➜  demos git:(main) ✗ ulimit -s
8192
```

The space allocated to the stack is provided a high address in the processes memory layout.  This allows the stack to grow down (towards the heap). In the `mem-automatic` demo program we can inspect the stack pointer using the following command in vscode:

```bash
-exec print $sp
$1 = (void *) 0xffffffffed50
```

This is a very large 48-bit number, as linux uses 48-bit virtual addresses on 64-bit machine architectures.  Lets use `objdump` to disassemble the first part of `fn()`:

```bash
➜  mem-basics git:(main) ✗ objdump -d ./mem-automatic | grep -A 20 "<fn>"
00000000000008d8 <fn>:
 8d8:   d10103ff        sub     sp, sp, #0x40
 8dc:   a9037bfd        stp     x29, x30, [sp, #48]
 8e0:   9100c3fd        add     x29, sp, #0x30
 8e4:   f90007e0        str     x0, [sp, #8]
```
We see that we are allocating 40 bytes onto the stack with the `sub` instruction.  This is because we have 5 local variables, each is a 64-bit value.  The above is from my machine which is ARM based.  You will also notice the `stp` (store pair) instruction that stores registers `x29` (the frame-pointer address) and `x30` (the function return address) on the stack. 

If we go to the bottom of the `fn()` function, we see the following:

```bash
 9ec:   a9437bfd        ldp     x29, x30, [sp, #48]
 9f0:   910103ff        add     sp, sp, #0x40
 9f4:   d65f03c0        ret
 ```
 This is where the stack gets restored. 

 The other thing that is important to know is that unlike `statically` allocated variables, local variables on the stack are never given a specific address.  After a stack frame is created for a function, all references to **local variables** are just taken using offsets from the `sp` register.  For example we can see this in this instruction:

 ```bash
  9d0:   f94017e3        ldr     x3, [sp, #40]
  ```

  To show the concepts shown above, lets take a look at the `messedUp()` function to see how we can manipulate variables without referencing them by knowing where they are at on the stack.



