CFLAGS= -Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS=

.PHONY:all

all:bme vm

ebasm : main.c vm.c
    $(CC) $(CFLAGS) -o vm main.c $(LIBS)

bme: bme.c  vm.c
    $(CC) $(CFLAGS) -o vm main.c $(LIBS)

.PHONY:examples
examples:./examples/fib.bm ./examples/123.bm

./examples/fib.bm : %.ebasm
    ./ebasm ./examples/fib.ebasm ./examples/fib.bm

./examples/123.bm : %.ebasm
    ./ebasm ./examples/123.ebasm ./examples/123.bm