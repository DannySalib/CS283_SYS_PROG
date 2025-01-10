CC=gcc
CFLAGS=-Wall -g

# Default target
all: cl_demo

# Static build target
static: LDFLAGS=-static
static: CFLAGS+=-static
static: cl_demo

cl_demo: cl_demo.o math_stuff.o
	$(CC) $(LDFLAGS) -o cl_demo cl_demo.o math_stuff.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f cl_demo *.o

#don'd delete the intermediate object files by default
.PRECIOUS: %.o