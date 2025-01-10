## CL_DEMO

#### Introduction

1. To build run `make` or `make static`, the makefile should do everything.  You can also run `make clean` to clean things up.

2. To start understanding things lets look at both the statically and dynamically linked version fo this program by running `make static` and `make`.

- Running `make`:
    ```bash
    bsm23@code:~/SysProg-Class/demos/elf-comp-link$ touch *.c
    bsm23@code:~/SysProg-Class/demos/elf-comp-link$ make
    gcc -Wall -g -c cl_demo.c
    gcc -Wall -g -c math_stuff.c
    gcc  -o cl_demo cl_demo.o math_stuff.o
    bsm23@code:~/SysProg-Class/demos/elf-comp-link$ ls -l ./cl_demo
    -rwxr-xr-x 1 bsm23 bsm23 72600 Jan  2 08:56 ./cl_demo
    ```
- Running `make static`:
    ```bash
    bsm23@code:~/SysProg-Class/demos/elf-comp-link$ touch *.c
    bsm23@code:~/SysProg-Class/demos/elf-comp-link$ make static
    gcc -Wall -g -c cl_demo.c
    gcc -Wall -g -c math_stuff.c
    gcc -static -o cl_demo cl_demo.o math_stuff.o
    bsm23@code:~/SysProg-Class/demos/elf-comp-link$ ls -l ./cl_demo
    -rwxr-xr-x 1 bsm23 bsm23 642136 Jan  2 09:01 ./cl_demo
    ```
    Notice that the statically linked version of the program comes in at 642,128 bytes, compared with the standard (dynamically) linked version that comes in much smaller at 72,600 bytes.  This is almost `9x` smaller.  We will get into this shortly.


#### Static Linked Deep Dive

For this part of the demo we will be looking into the statically linked version of this program.  Run `touch *.c` and then `make static`.  Now lets dig into what we have:

1.  Lets examine the ELF headers:
    ```bash
    readelf -h ./cl_demo.o
    readelf -h ./math_stuff.o
    ```
    Notice both of these files have the type of `REL(Relocatable file)` with an `Entry point address = 0`.  ELF files that are of type `REL` cannot be executed.

2. Now lets look at the sections, specifically the symbol table (`-s`):
    ```bash
    readelf -s ./cl_demo.o
    readelf -s ./math_stuff.o
    ```
    Notice that for `cl_demo.o` (im eliminating the gcc generated symbols):
    ```bash
    Symbol table '.symtab' contains 24 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS cl_demo.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 .bss
     5: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    1 $x
     6: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     7: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    5 $d
     8: 0000000000000000     0 SECTION LOCAL  DEFAULT    6 .debug_info
     9: 0000000000000000     0 SECTION LOCAL  DEFAULT    8 .debug_abbrev
    10: 0000000000000000     0 SECTION LOCAL  DEFAULT    9 .debug_aranges
    11: 0000000000000000     0 SECTION LOCAL  DEFAULT   11 .debug_line
    12: 0000000000000000     0 SECTION LOCAL  DEFAULT   13 .debug_str
    13: 0000000000000000     0 SECTION LOCAL  DEFAULT   14 .debug_line_str
    14: 0000000000000000     0 SECTION LOCAL  DEFAULT   16 .note.GNU-stack
    15: 0000000000000014     0 NOTYPE  LOCAL  DEFAULT   17 $d
    16: 0000000000000000     0 SECTION LOCAL  DEFAULT   17 .eh_frame
    17: 0000000000000000     0 SECTION LOCAL  DEFAULT   15 .comment
    18: 0000000000000000    44 FUNC    GLOBAL DEFAULT    1 isodd
    19: 000000000000002c   124 FUNC    GLOBAL DEFAULT    1 main
    20: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND my_add
    21: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
    22: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND puts
    23: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND exit
    ```

    All of the symbols (aka functions) that are included in this file like `main()` and `isodd()` has a size, and the `1` means that the implementation is in Section 1 of the ELF file.  If you go to the top you will see that `SECTION 1` is the `.text` section.  This is where the machine code for these functions is located.  Notice that the functions `my_add()`, `printf()`, `puts()`, and `exit()` are marked as `UND` or undefined. Why?  Because these functions are not present in this object file, they will have to be resolved later by the linking process.

    **sidebar** where did `puts()` come from, we never call that function.  This is a gcc "optimization".  When we call `printf()`, the `printf()` function in the C runtime library is called if we use any sort of formatting in the call.  We did this here: `printf("The value of %d + %d is %d\n", a,b,c);`.  However, if we call `printf()` without any formatting the compiler replaces it with a simpler and faster call to `puts()` like we did here: `printf("The result is odd\n");`.  Back to the demo...

3.  Now lets disassemble main by running `objdump -d ./cl_demo.o`.  I am running on an ARM-based machine:

    ```bash
    000000000000002c <main>:
    2c:   a9be7bfd        stp     x29, x30, [sp, #-32]!
    30:   910003fd        mov     x29, sp
    34:   528000a0        mov     w0, #0x5                        // #5
    38:   b90017e0        str     w0, [sp, #20]
    3c:   52800280        mov     w0, #0x14                       // #20
    40:   b9001be0        str     w0, [sp, #24]
    44:   b9401be1        ldr     w1, [sp, #24]
    48:   b94017e0        ldr     w0, [sp, #20]
    4c:   94000000        bl      0 <my_add>
    50:   b9001fe0        str     w0, [sp, #28]
    54:   b9401fe3        ldr     w3, [sp, #28]
    58:   b9401be2        ldr     w2, [sp, #24]
    5c:   b94017e1        ldr     w1, [sp, #20]
    60:   90000000        adrp    x0, 0 <isodd>
    64:   91000000        add     x0, x0, #0x0
    68:   94000000        bl      0 <printf>
    6c:   b9401fe0        ldr     w0, [sp, #28]
    70:   94000000        bl      0 <isodd>
    74:   39004fe0        strb    w0, [sp, #19]
    78:   39404fe0        ldrb    w0, [sp, #19]
    7c:   12000000        and     w0, w0, #0x1
    80:   7100001f        cmp     w0, #0x0
    84:   540000a0        b.eq    98 <main+0x6c>  // b.none
    88:   90000000        adrp    x0, 0 <isodd>
    8c:   91000000        add     x0, x0, #0x0
    90:   94000000        bl      0 <puts>
    94:   14000004        b       a4 <main+0x78>
    98:   90000000        adrp    x0, 0 <isodd>
    9c:   91000000        add     x0, x0, #0x0
    a0:   94000000        bl      0 <puts>
    a4:   52800000        mov     w0, #0x0                        // #0
    a8:   94000000        bl      0 <exit>
    ```

    Lets factor out the function calls, in ARM the instruction is `bl` that has the opcode `100101`:

    ```bash
    000000000000002c <main>:
    4c:   94000000        bl      0 <my_add>
    68:   94000000        bl      0 <printf>
    70:   94000000        bl      0 <isodd>
    90:   94000000        bl      0 <puts>
    a0:   94000000        bl      0 <puts>
    a8:   94000000        bl      0 <exit>
    ```

    The machine code for all of these calls is exactly the same thing: `0x94000000`.  In binary that is `0b100101 <26 more zeros>`.  Thus, the opcode is `bl (100101)` but the jump address is unknown so its replaced with all zeros.  If you have an Intel machine, or run on tux, the same thing happens, but this time the assembler instruction is `call` vs `bl`.  Running on tux, all of the function calls in `cl_demo.o`:

    ```bash
    000000000000002c <main>:
    47:	e8 00 00 00 00       	call   4c <main+0x29>
    69:	e8 00 00 00 00       	call   6e <main+0x4b>
    73:	e8 00 00 00 00       	call   78 <main+0x55>
    8b:	e8 00 00 00 00       	call   90 <main+0x6d>
    9c:	e8 00 00 00 00       	call   a1 <main+0x7e>
    a6:	e8 00 00 00 00       	call   ab <main+0x88>
    ```
    Just like the ARM version, the machine code for Intel/AMD is `e6` for the `call` opcode and all zeros for where to branch because its undefined in a `REL` ELF file (the format for .o object files).

4.  Now lets look at the linked version `cl_demo`.  Since this file is statically linked, the linker basically combines all of the `.text` and `.data` sections to create an executable ELF file.  Running:

    ```bash
    ➜  elf-comp-link git:(main) readelf -h ./cl_demo      
    ELF Header:
    Magic:   7f 45 4c 46 02 01 01 03 00 00 00 00 00 00 00 00 
    Class:                             ELF64
    Data:                              2's complement, little endian
    Version:                           1 (current)
    OS/ABI:                            UNIX - GNU
    ABI Version:                       0
    Type:                              EXEC (Executable file)
    Machine:                           AArch64
    Version:                           0x1
    Entry point address:               0x400640
    Start of program headers:          64 (bytes into file)
    Start of section headers:          640088 (bytes into file)
    Flags:                             0x0
    Size of this header:               64 (bytes)
    Size of program headers:           56 (bytes)
    Number of program headers:         6
    Size of section headers:           64 (bytes)
    Number of section headers:         32
    Section header string table index: 31
    ```
    Notice this time the ELF file type is `TYPE EXEC`.

5.  Remember the `UDF` undefined symbols from the object file, lets look at what happened to them.  Running `readelf -s ./cl_demo | more` shows that we now have a lot of symbols, 3059 to be exact.  Many of these come from the C runtime library.  To filter out the ones that we want we use `grep`.  For example: `readelf -s ./cl_demo | grep "\bmain"` will look for the `main` function symbol. The current situation is shown below:

    ```bash
    Symbol table '.symtab' contains 3059 entries:
     Num:    Value          Size Type    Bind   Vis      Ndx Name
       6: 00000000004002c0     0 SECTION LOCAL  DEFAULT    6 .text
    2179: 00000000004007b0    44 FUNC    GLOBAL DEFAULT    6 isodd
    2624: 00000000004007dc   128 FUNC    GLOBAL DEFAULT    6 main
    2636: 000000000040085c    40 FUNC    GLOBAL DEFAULT    6 my_add
    1938: 0000000000401680   196 FUNC    GLOBAL DEFAULT    6 printf
    2133: 0000000000405a70   584 FUNC    WEAK   DEFAULT    6 puts
    2875: 0000000000401660    28 FUNC    GLOBAL HIDDEN     6 exit
    ```

    Take a look at the value for main, its `0x00000000004007dc`. Now, lets look at the disassembled code for `main()` in the executable by running `objdump -d --disassemble=main ./cl_demo`.  **NOTICE that the first instruction in main is at location `0x00000000004007dc`, the same value in the symbol table.**

    ```bash
    00000000004007dc <main>:
    4007dc:       a9be7bfd        stp     x29, x30, [sp, #-32]!
    4007e0:       910003fd        mov     x29, sp
    4007e4:       528000a0        mov     w0, #0x5                        // #5
    4007e8:       b90017e0        str     w0, [sp, #20]
    4007ec:       52800280        mov     w0, #0x14                       // #20
    4007f0:       b9001be0        str     w0, [sp, #24]
    4007f4:       b9401be1        ldr     w1, [sp, #24]
    4007f8:       b94017e0        ldr     w0, [sp, #20]
    4007fc:       94000018        bl      40085c <my_add>
    400800:       b9001fe0        str     w0, [sp, #28]
    400804:       b9401fe3        ldr     w3, [sp, #28]
    400808:       b9401be2        ldr     w2, [sp, #24]
    40080c:       b94017e1        ldr     w1, [sp, #20]
    400810:       900002a0        adrp    x0, 454000 <__gcc_personality_v0+0x3c>
    400814:       9111a000        add     x0, x0, #0x468
    400818:       9400039a        bl      401680 <_IO_printf>
    40081c:       b9401fe0        ldr     w0, [sp, #28]
    400820:       97ffffe4        bl      4007b0 <isodd>
    400824:       39004fe0        strb    w0, [sp, #19]
    400828:       39404fe0        ldrb    w0, [sp, #19]
    40082c:       12000000        and     w0, w0, #0x1
    400830:       7100001f        cmp     w0, #0x0
    400834:       540000a0        b.eq    400848 <main+0x6c>  // b.none
    400838:       900002a0        adrp    x0, 454000 <__gcc_personality_v0+0x3c>
    40083c:       91122000        add     x0, x0, #0x488
    400840:       9400148c        bl      405a70 <_IO_puts>
    400844:       14000004        b       400854 <main+0x78>
    400848:       900002a0        adrp    x0, 454000 <__gcc_personality_v0+0x3c>
    40084c:       91128000        add     x0, x0, #0x4a0
    400850:       94001488        bl      405a70 <_IO_puts>
    400854:       52800000        mov     w0, #0x0                        // #0
    400858:       94000382        bl      401660 <exit>
    ```

6. Next, lets see what the linker did to the function calls.  Filtering out the function calls, aka the `bl`:

    ```bash
    00000000004007dc <main>:
    4007fc:       94000018        bl      40085c <my_add>
    400818:       9400039a        bl      401680 <_IO_printf>
    400820:       97ffffe4        bl      4007b0 <isodd>
    400840:       9400148c        bl      405a70 <_IO_puts>
    400850:       94001488        bl      405a70 <_IO_puts>
    400858:       94000382        bl      401660 <exit>
    ```
    This time notice that all of the machine code for the `bl` instruction still starts with `100101` however the remaining zeros were replaced with offsets to where these functions are actually located.

    Lets dig into a few example. Lets look at the call to the `isodd()` function.  From above:

    ```
    400820:       97ffffe4        bl      4007b0 <isodd>
    ```
    Now lets disassemble `isodd()` using `objdump -d --disassemble=isodd ./cl_demo`:

    ```bash
    00000000004007b0 <isodd>:
    4007b0:       d10043ff        sub     sp, sp, #0x10
    4007b4:       b9000fe0        str     w0, [sp, #12]
    4007b8:       b9400fe0        ldr     w0, [sp, #12]
    4007bc:       12000000        and     w0, w0, #0x1
    4007c0:       7100001f        cmp     w0, #0x0
    4007c4:       54000060        b.eq    4007d0 <isodd+0x20>  // b.none
    4007c8:       52800020        mov     w0, #0x1                        // #1
    4007cc:       14000002        b       4007d4 <isodd+0x24>
    4007d0:       52800000        mov     w0, #0x0                        // #0
    4007d4:       910043ff        add     sp, sp, #0x10
    4007d8:       d65f03c0        ret
    ```
    Notice the `isodd()` function starts at position `0x4007b0`. And in `main()` the `bl` instrucion to call `isodd()`

    ```
    400820:       97ffffe4        bl      4007b0 <isodd>
    ```

    Thus the linker replaced the address of the code generated in the object file `bl 0` with `bl 4007b0` which is the address of the implementation.  If we do this for other functions like `my_add()` via `objdump -d --disassemble=my_add ./cl_demo` we will see the same behavior, the `my_add()` function was resolved by the linker with its code starting at `0x40085c`.  We will see this for all of the functions, even the ones in the C runtime library since we dynamically linked this version of the executable.

7.  So what are all of these addresses? They are relitive addresses based on assuming the operating system loads the program starting at memory address 0.  More on this later.  But for now, look at the ELF header for the executable again by running `readelf -h ./cl_demo`.  This time look at the `Entry point address:` field:

    ```
    Entry point address:               0x400640
    ```

    What exactly is located there?  Run `objdump --disassemble=_start ./cl_demo`. Notice the ELF file instructs the program to bootstrap by running the `_start()` function. This function does a bunch of things to setup the C runtime library.  The last thing that it does is to branch to `main()` via `b  4007dc`.  From above that is exactly where main is located in this executable. 

    ```
    0000000000400640 <_start>:
    400640:       d503201f        nop
    400644:       d280001d        mov     x29, #0x0                       // #0
    400648:       d280001e        mov     x30, #0x0                       // #0
    40064c:       aa0003e5        mov     x5, x0
    400650:       f94003e1        ldr     x1, [sp]
    400654:       910023e2        add     x2, sp, #0x8
    400658:       910003e6        mov     x6, sp
    40065c:       90000000        adrp    x0, 400000 <__ehdr_start>
    400660:       9119d000        add     x0, x0, #0x674
    400664:       d2800003        mov     x3, #0x0                        // #0
    400668:       d2800004        mov     x4, #0x0                        // #0
    40066c:       940000c2        bl      400974 <__libc_start_main>
    400670:       97ffff14        bl      4002c0 <abort>

    0000000000400674 <__wrap_main>:
    400674:       d503201f        nop
    400678:       14000059        b       4007dc <main>
    ```

    Getting back to all of these addresses.  Linux uses a position independent code model.  The executable generates all addresses assuming the program is loaded at memory location 0.  If that was the case the `_start()` function to get things running would be positioned in memory location `0x400640`. 

    Linux uses a special security feature called Address Space Layout Randomization (ASLR).  You can see if this is enabled by `cat /proc/sys/kernel/randomize_va_space`.  If the value returned is `0` ASLR is disabled, `2` is fully enabled, and `1` is partially enabled. 

#### Dynamic Linking Deep Dive

To better understand dynamic linking we will start off with a simpler program called `dl_printf.c`.  The code is below:

```c
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
```
Note the commands to compile and link the program are shown as comments. We will be examining both the object file and executable moving forward.  Also notice the reference to `-no-pie` in the compile steps.  We will talk about this later in the course, but for for now it eliminates a security feature in Linux called Address Space Layout Randomization (ASLR), by eliminating the default compiler option to create a **position independent executable**. Eliminating this feature will make demonstrating and showing how dynamic linking works much easier.  

The objective of this is to understand how the implementation for the `printf()` function gets resolved.  This function is part of `libc` and by default is dynamically linked into your program.

Dynamic linking is used by just about every modern operating system.  In windows they are called **dynamic link libraries** or DLLs.  In Linux they are called **shared objects** and are generally represented by files with `.so` extensions.  Unlike static linking, where the linker resolves all symbols in advance of execution, dynamic linking resolves symbols at runtime dynamically.  

There are many advantages of dynamic linking (along with some disadvantages).  The main benefits are:

1.  (Advantage) The executable file can be smaller, given the dynamically linked code is not included in the executable file itself.
2.  (Advantage) The memory footprint is smaller.  Consider the `printf()` function in `libc`.  If many programs are using it, only one copy has to be in memory that is shared across every program.
3. (Disadvantage)  A statically linked executable, is by definition **static**  all of the code that is included at compile time and will never change, so behavior will be predictable always.
4. (Disadvantage) Building on 3, an update to a dynamic link library that introduces a bug or a subtle change can break many things at one time.  One day your program is running fine, then a system update happens, and now it no longer runs fine. This is very hard to debug and/or troubleshoot. 

If we examine the object file we see the same thing we saw before with static linking:

```bash
readelf -s ./dl_printf.o

Symbol table '.symtab' contains 20 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS dl_printf.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 .bss
     5: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     6: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    5 $d
     7: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    1 $x
     8: 0000000000000000     0 SECTION LOCAL  DEFAULT    6 .debug_info
     9: 0000000000000000     0 SECTION LOCAL  DEFAULT    8 .debug_abbrev
    10: 0000000000000000     0 SECTION LOCAL  DEFAULT    9 .debug_aranges
    11: 0000000000000000     0 SECTION LOCAL  DEFAULT   11 .debug_line
    12: 0000000000000000     0 SECTION LOCAL  DEFAULT   13 .debug_str
    13: 0000000000000000     0 SECTION LOCAL  DEFAULT   14 .debug_line_str
    14: 0000000000000000     0 SECTION LOCAL  DEFAULT   16 .note.GNU-stack
    15: 0000000000000014     0 NOTYPE  LOCAL  DEFAULT   17 $d
    16: 0000000000000000     0 SECTION LOCAL  DEFAULT   17 .eh_frame
    17: 0000000000000000     0 SECTION LOCAL  DEFAULT   15 .comment
    18: 0000000000000000    72 FUNC    GLOBAL DEFAULT    1 main
    19: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
```
As expected, the `printf` function is tagged as undefined `UND`.  The main changes are observed in the executable:

The fist change is that there is an `.interp` section.

```bash
readelf -x .interp   ./dl_printf

Hex dump of section '.interp':
  0x00400238 2f6c6962 2f6c642d 6c696e75 782d6161 /lib/ld-linux-aa
  0x00400248 72636836 342e736f 2e3100            rch64.so.1.
```
This section points to the location of the dynamic linker that will be used to resolve symbols dynamically.

Next, lets see which shared libraries are needed by this executable:

```bash
readelf -d ./dl_printf | 
grep NEEDED
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
```

We see that `libc.so.6` is needed, this is where the code for `printf` lives.  The ELF file also has version information for dynamic dependencies:

```bash
readelf -V ./dl_printf

Version symbols section '.gnu.version' contains 7 entries:
 Addr: 0x00000000004003e6  Offset: 0x000003e6  Link: 5 (.dynsym)
  000:   0 (*local*)       2 (GLIBC_2.34)    1 (*global*)      1 (*global*)   
  004:   3 (GLIBC_2.17)    1 (*global*)      3 (GLIBC_2.17) 

Version needs section '.gnu.version_r' contains 1 entry:
 Addr: 0x00000000004003f8  Offset: 0x000003f8  Link: 6 (.dynstr)
  000000: Version: 1  File: libc.so.6  Cnt: 2
  0x0010:   Name: GLIBC_2.17  Flags: none  Version: 3
  0x0020:   Name: GLIBC_2.34  Flags: none  Version: 2
```

Now, like with the object file, lets look at the symbols, we notice something has changed:

```bash
readelf -s ./dl_printf | grep printf
     6: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND printf@GLIBC_2.17 (3)
    63: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS dl_printf.c
    96: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND printf@GLIBC_2.17
```
Now we see that the references to `printf` being undefined in the object file are now `printf@GLIBC_2.17`.  This symbol is also marked as undefined.

What we know now:

1. The executable `ELF` file has a reference to the linux dynamic linker
2. There is a `.dynamic` section in the elf file that highlights that the `libc.so.6` file is `NEEDED`.
3. There is a `.gnu.version_r` section that introduces the label `GLIBC_2.17` and associates it with `libc.so.6`
4. The `UND` references to `printf` in the object file have been changed to `printf@GLIBC_2.17` in the executable file.

With this knowledge, lets see how dynamic linking works at runtime. To do this we will be using the linux debugger `gdb`. Start the debugging process by running `gdb ./dl_printf`.  Now set things up by executing the following `gdb` commands:

```
set disassemble-next-line on
break _start
break main
run
```
We should see output that we are at the breakpoint for `_start` which we have talked about before, this is where the C runtime library does some initialization prior to calling `main()`.  Lets see what dynamic libraries are loaded prior to our program even starting to execute:

```bash
(gdb) info sharedlibrary
From                To                  Syms Read   Shared Object Library
0x0000fffff7fbedc0  0x0000fffff7fdae70  Yes         /lib/ld-linux-aarch64.so.1
```

We see that the linux loader loaded the dynamic linker.  It did this based on the data in the `.interop` section we looked at earlier.  Now lets head over to main by executing the `continue` command.

```bash
(gdb) continue
Continuing.

Breakpoint 2, main () at dl_printf.c:7
warning: Source file is more recent than executable.
7
=> 0x00000000004006b0 <main+8>: 52800020        mov     w0, #0x1                         // #1
   0x00000000004006b4 <main+12>:        b9001fe0        str     w0, [sp, #28]
```

Now we are in `main`, lets see what libraries are loaded now, this is one of the services provided by `_start()` bootstraping the dynamic linker.  Lets run `info sharedlibrary` again:

```bash
(gdb) info sharedlibrary
From                To                  Syms Read   Shared Object Library
0x0000fffff7fbedc0  0x0000fffff7fdae70  Yes         /lib/ld-linux-aarch64.so.1
0x0000fffff7e17d00  0x0000fffff7f341f0  Yes         /lib/aarch64-linux-gnu/libc.so.6
```
We now see that the `libc` dynamic library has been loaded.  Now lets step to the first call to `printf` using the `stepi` command.  Note that the call is to a function named `<printf@plt>`.  Lets disassemble this function:

```bash
(gdb) disassemble 'printf@plt'
Dump of assembler code for function printf@plt:
   0x0000000000400540 <+0>:     adrp    x16, 0x420000 <__libc_start_main@got.plt>
   0x0000000000400544 <+4>:     ldr     x17, [x16, #24]
   0x0000000000400548 <+8>:     add     x16, x16, #0x18
   0x000000000040054c <+12>:    br      x17
```
This is kind of interesting.  This function first gets the address of the root of the `.plt` table.  On ARM it assigns this value to register `x16`.  It then adds 24 in decimal or 0x18 to this value and loads the contents of that memory location inside of register `x17`.  It also updates the address in register `x16` by adding 24 as well.  Thus register `x16` contains an address and register `x17` contains the value at that address.  Lets add a breakpoint at this function now by executing `break 'printf@plt'`, and then running `continue`. Lets step through this function to right before the `br` call:

```bash
(gdb) break 'printf@plt'
Breakpoint 3 at 0x400540
(gdb) continue
Continuing.

Breakpoint 3, 0x0000000000400540 in printf@plt ()
=> 0x0000000000400540 <printf@plt+0>:   90000110        adrp    x16, 0x420000 <__libc_start_main@got.plt>
(gdb) stepi
0x0000000000400544 in printf@plt ()
=> 0x0000000000400544 <printf@plt+4>:   f9400e11        ldr     x17, [x16, #24]
(gdb) stepi
0x0000000000400548 in printf@plt ()
=> 0x0000000000400548 <printf@plt+8>:   91006210        add     x16, x16, #0x18
(gdb) stepi
0x000000000040054c in printf@plt ()
=> 0x000000000040054c <printf@plt+12>:  d61f0220        br      x17
```
Lets inspect the values in registers `x16` and `x17`:

```bash
(gdb) p/x $x16
$1 = 0x420018
(gdb) p/x $x17
$2 = 0x4004f0
(gdb)
```

So memory location 0x420018 contains the value 0x4004f0.  This means that we will be branching to memory location 0x4004f0.  But what is there?  One way is to use `objdump`:

```bash
objdump -d -j .plt ./dl_printf

./dl_printf:     file format elf64-littleaarch64


Disassembly of section .plt:

00000000004004f0 <.plt>:
  4004f0:       a9bf7bf0        stp     x16, x30, [sp, #-16]!
  4004f4:       f00000f0        adrp    x16, 41f000 <__FRAME_END__+0x1e7c0>
  4004f8:       f947fe11        ldr     x17, [x16, #4088]
  4004fc:       913fe210        add     x16, x16, #0xff8
  400500:       d61f0220        br      x17
  400504:       d503201f        nop
  400508:       d503201f        nop
  40050c:       d503201f        nop

0000000000400510 <__libc_start_main@plt>:
  400510:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400514:       f9400211        ldr     x17, [x16]
  400518:       91000210        add     x16, x16, #0x0
  40051c:       d61f0220        br      x17

0000000000400520 <__gmon_start__@plt>:
  400520:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400524:       f9400611        ldr     x17, [x16, #8]
  400528:       91002210        add     x16, x16, #0x8
  40052c:       d61f0220        br      x17

0000000000400530 <abort@plt>:
  400530:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400534:       f9400a11        ldr     x17, [x16, #16]
  400538:       91004210        add     x16, x16, #0x10
  40053c:       d61f0220        br      x17

0000000000400540 <printf@plt>:
  400540:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
  400544:       f9400e11        ldr     x17, [x16, #24]
  400548:       91006210        add     x16, x16, #0x18
  40054c:       d61f0220        br      x17
```
Notice the root of the `.plt` table is at address `0x4004f0` so we will be jumping there.  We can also see this in `gdb`:

```bash
disassemble 0x4004f0, 0x400510
Dump of assembler code from 0x4004f0 to 0x400510:
=> 0x00000000004004f0:  stp     x16, x30, [sp, #-16]!
   0x00000000004004f4:  adrp    x16, 0x41f000
   0x00000000004004f8:  ldr     x17, [x16, #4088]
   0x00000000004004fc:  add     x16, x16, #0xff8
   0x0000000000400500:  br      x17
   0x0000000000400504:  nop
   0x0000000000400508:  nop
   0x000000000040050c:  nop
End of assembler dump.
```
If we step through this a few times we will find ourselves in a function called `_dl_runtime_resolve`, which is part of the dynamic linker:

```bash
(gdb) stepi
0x00000000004004f4 in ?? ()
=> 0x00000000004004f4:  f00000f0        adrp    x16, 0x41f000
(gdb) stepi
0x00000000004004f8 in ?? ()
=> 0x00000000004004f8:  f947fe11        ldr     x17, [x16, #4088]
(gdb) stepi
0x00000000004004fc in ?? ()
=> 0x00000000004004fc:  913fe210        add     x16, x16, #0xff8
(gdb) stepi
0x0000000000400500 in ?? ()
=> 0x0000000000400500:  d61f0220        br      x17
(gdb) stepi
_dl_runtime_resolve () at ../sysdeps/aarch64/dl-trampoline.S:38
warning: 38     ../sysdeps/aarch64/dl-trampoline.S: No such file or directory
=> 0x0000fffff7fce74c <_dl_runtime_resolve+0>:  d503201f        nop
(gdb) stepi
50      in ../sysdeps/aarch64/dl-trampoline.S
=> 0x0000fffff7fce750 <_dl_runtime_resolve+4>:  a9b327e8        stp     x8, x9, [sp, #-208]!
(gdb) 
```
Remember we set a breakpoint earlier at `printf@plt`.  This is what gets called when `printf()` is executed.  We are currently in our first execution of `printf()` where we are somewhere inside of the dynamic linking process.  Lets let that finish, handle the `printf()` and continue to the second call of `printf()` in our code.  We do this by executing `continue`:

```bash
(gdb) continue
Continuing.
Calling printf - time 1

Breakpoint 3, 0x0000000000400540 in printf@plt ()
=> 0x0000000000400540 <printf@plt+0>:   90000110        adrp    x16, 0x420000 <__libc_start_main@got.plt>
```
Notice the output from the first call to `printf()` is displayed.  We are now at the breakpoing for the second call.  Lets step through this again:

```bash
(gdb) stepi
0x0000000000400544 in printf@plt ()
=> 0x0000000000400544 <printf@plt+4>:   f9400e11        ldr     x17, [x16, #24]
(gdb) stepi
0x0000000000400548 in printf@plt ()
=> 0x0000000000400548 <printf@plt+8>:   91006210        add     x16, x16, #0x18
(gdb) stepi
0x000000000040054c in printf@plt ()
=> 0x000000000040054c <printf@plt+12>:  d61f0220        br      x17
(gdb) 
```
Notice, as expected this is the exact same code as before.  We are now setup to branch to the value in register `x17` that is in memory at the address held in register `x16`.  Lets inspect these values:

```bash
(gdb) p/x $x16
$3 = 0x420018
(gdb) p/x $x17
$4 = 0xfffff7e40590
(gdb) 
```
This is where we see the dynamic linking magic.  On both the first and second execution of `printf@plt` we calculate an address in the `got` table that is in register `x16`.  This value has not changed.  However, the value in that memory location (in `x17`) has changed.  The first time it was `0x4004f0` Which was the address of the root of the `.plt` table which is where the dynamic linker is invoked.  This time its showing a totally different memory address somewhere far away from the current program counter, address `0xfffff7e40590`.  Lets disassemble what is there:

```bash
(gdb) disassemble 0xfffff7e40590
Dump of assembler code for function __printf:
   0x0000fffff7e40590 <+0>:     sub     sp, sp, #0x120
   0x0000fffff7e40594 <+4>:     mov     w12, #0xffffffc8                // #-56
   0x0000fffff7e40598 <+8>:     adrp    x9, 0xfffff7f9f000 <_nl_C_locobj+32>
   0x0000fffff7e4059c <+12>:    ldr     x9, [x9, #3720]
   0x0000fffff7e405a0 <+16>:    add     x13, sp, #0xe0
   0x0000fffff7e405a4 <+20>:    stp     x29, x30, [sp, #80]
   0x0000fffff7e405a8 <+24>:    add     x29, sp, #0x50
   0x0000fffff7e405ac <+28>:    mov     w11, #0xffffff80                // #-128
   0x0000fffff7e405b0 <+32>:    adrp    x10, 0xfffff7f9f000 <_nl_C_locobj+32>
   0x0000fffff7e405b4 <+36>:    ldr     x10, [x10, #3600]
--Type <RET> for more, q to quit, c to continue without paging--
```
What?  Super cool, This is the code for the real `printf()` function.  We can step a few instructions into it (its a long function), using stepi, and when we get bored we can just `continue`: 

```bash
(gdb) stepi
__printf (format=0x400710 "Calling printf - time %d\n")
    at ./stdio-common/printf.c:28
warning: 28     ./stdio-common/printf.c: No such file or directory
=> 0x0000fffff7e40590 <__printf+0>:     d10483ff        sub     sp, sp, #0x120
(gdb) stepi
0x0000fffff7e40594      32      in ./stdio-common/printf.c
=> 0x0000fffff7e40594 <__printf+4>:     128006ec        mov     w12, #0xffffffc8                 // #-56
(gdb) stepi
0x0000fffff7e40598      28      in ./stdio-common/printf.c
=> 0x0000fffff7e40598 <__printf+8>:     f0000ae9        adrp    x9, 0xfffff7f9f000 <_nl_C_locobj+32>
   0x0000fffff7e4059c <__printf+12>:    f9474529        ldr     x9, [x9, #3720]
(gdb) continue
Continuing.
Calling printf - time 2
[Inferior 1 (process 112176) exited normally]
```
After we get bored and execute `continue` we will see the output from the second call to `printf()` in our source, and then the program exits.

**RECAP**

1. Dynamic linking is enabled by several artifacts that are placed into the executable ELF file.  These include the PLT, the GOT and references to where the dynamic linker is located along with what requires dynamic linking.

2. The C standard library bootstraps the dynamic linking process in `_start()`.  We saw that the C runtime library was not part of the memory layout of our test program until after `_start()` executed and we were in our own `main()` function.

3. Dynamic linking uses clever indirection and a lazy approach.  The first time a call to a dynamically linked function is invoked, the code calls the dynamic linker that locates the exact entry point into the function, updates the GOT address, and calls the function.  The next time (and every other time) the function is called, the dynamic linker is bypassed becuase it updated the address to the function in the GOB.

4. Even with the one-time overhead of the first call dynamic linking is always less efficient that static linking.  With static linking, functions are called using 1 instruction.  On ARM it will be called using `bl <register with address of function>`.  With dynamic linking we always call the same code that is in the PLT, recall `printf@plt`:

    ``` bash
    0000000000400540 <printf@plt>:
    400540:       90000110        adrp    x16, 420000 <__libc_start_main@GLIBC_2.34>
    400544:       f9400e11        ldr     x17, [x16, #24]
    400548:       91006210        add     x16, x16, #0x18
    40054c:       d61f0220        br      x17
    ```
    Notice how calling `printf()` using dynamic linking will always require 4 instructions after the first call.  This is because the code for `printf@plt` cannot be modified.  What gets modified is the address in the GOT that `printf@plt` calls. 