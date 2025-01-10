## Static Memory Storage

compile with:

```bash
gcc -g -o mem-static mem-static.c
```

Note here that we are talking about statically allocated variables and not the `static` keyword in C.  Storage for all statically allocated variables is defined inside of the executable itself.  We will get into this in detail later, but for now its important to know that all of the different flavors of statically allocated variables are shown in this demonstration:

    1. *Global* variables
    2. *File/Module-Scoped* variables (using the `static` keyword)
    3. *Uninitialized* variables, or variables initialized to zero
    4. *Constant* variables

The key point of this demo is that the executable itself not only contains the binary code for the program, but also contains all of the information about statically allocated variables that the loader uses to create space when the program is brought into memory.

To see some of this in action, lets use the `readelf` utility: 

```bash
bsm23@code:~/SysProg-Class/demos/mem-basics$ readelf -s ./mem-static | grep "\bd_"
    62: 0000000000020028     4 OBJECT  LOCAL  DEFAULT   23 d_stat_not_init
    63: 000000000002002c     4 OBJECT  LOCAL  DEFAULT   23 d_stat_zero_init
    64: 0000000000020014     4 OBJECT  LOCAL  DEFAULT   22 d_stat_init
    66: 000000000000086c     4 OBJECT  LOCAL  DEFAULT   15 d_const_stat_init
    83: 0000000000020010     4 OBJECT  GLOBAL DEFAULT   22 d_glb_init
    94: 0000000000000868     4 OBJECT  GLOBAL DEFAULT   15 d_const_init
   100: 0000000000020024     4 OBJECT  GLOBAL DEFAULT   23 d_glob_zero_init
   101: 0000000000020020     4 OBJECT  GLOBAL DEFAULT   23 d_glob_not_init
```

Note there is a lot of things in the executable, I named all variables to start with `d_` just to make them easier to filter.  If we parse what is returned we see a few things.

*  The 64 bit number is an *offset* to where storage is allocated for a particular variable relative to the starting address of the program.  We will not know where this is until Linux actually loads the program into memory. 

*   We next see `4` thats because all of our variables are defined as `int` which are 4 bytes on my machine.

*   The next column of interest is `LOCAL` or `GLOBAL`.  This defines the scope of the variable.  Note that keyword `static` in C sets the variable to have a `LOCAL` scope, with the default being `GLOBAL`.

*   The final column of interest is the one with the numbers in it.  Notice they are all `22`, `23` or `15`.  Lets next try `readelf -S -W ./mem-static`.  Notice that section `22` is called `.data` and has flags `WA` which means `read-write` and `allocate`.  Section 23 is called `.bss` and its the same as `.data`.  The `.bss` section is used for **ALL** uninitialized (or initialized to 0) varaibles.  Doing this enables the binary not to have to have space in it for these varaiable as they are created when the program loads.  Notice the `0x14`.  In decimal this is the value 20.  The sample program has 5 variables defined as `static` each is 4 bytes.  Finally section `15` is `.rodata`, this section is marked just with `A`, which means that it is allocated, but is also read-only.  

