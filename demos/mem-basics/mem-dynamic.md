## Dynamically Allocated Memory

We should already know that in C, you use the `malloc()` function to allocate dynamic memory, and the `free()` function to return the memory you allocated.

Well, this is not really what goes on from a systems programming perspective.  Consider the code:

```c
    char *ptr = malloc(64);
```

One would think that the operating system is *allocating* 64 bytes for you, but that would be super inefficient.  The Linux (and all other OS that I know about) work in memory units called *page size*.  Although configurable in Linux, most implementations default to 4K. 

```bash
bsm23@code:~/SysProg-Class/demos/mem-basics$ getconf PAGESIZE
4096
```

This means that Linux does not allocate bytes, it assigns 4K pages to processes as they need them.  The linux system calls `brk` and `sbrk`, along with `mmap` are used to get storage (in terms of pages) from the operating system.  Note that making system calls is a relatively slow operation, and in many cases we dont want or need our programs to work in 4K units.

To help with this the C runtime library (glibc), provides a library to make things easier for the programmer.  This library includes the functions `malloc()`, `free()`, `realloc()` and some others that we will talk about later.  

Recall that when your program first starts running, `main()` is not the first thing that is called.  Instead `gcc` provides a wrapper around `main()` called `_start()` to set things up and to clean up afterwords.  One of the things that happens before main even runs is that the c runtime library sets up the initial heap.  Although it can be configured and may change based on configuration, glibc creates the initial heap by preallocating 128K (or asking for 32 pages from the operating system).

So, what is really happening?

The C runtime library grabs an initial set of pages.  It then hands out and tracks memory from the pages that it manages when `malloc()` is called. It also will reuse memory after `free()` is called.  The dynamic memory functionality in `glibc` will also go out to the operating system and grab more pages if more space is needed, and will return it as it is freed up. 