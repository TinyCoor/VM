CFLAGS= -Wall -Wextra -Wswitch-enum -std=c11 -pedantic
CC = gcc

.PHONY:all

all:basm debasm bmi

# %.o :%.c
# 	$(CC) $(CFLAGS) -o ./build/$@ $<


debasm : ./src/disassmebler.c ./src/vm.c  ./src/string_view.c ./src/label.c		
	$(CC) $(CFLAGS) -o ./build/$@  $^ $(LIBS)

basm: ./src/main.c ./src/vm.c ./src/string_view.c ./src/label.c
	$(CC) $(CFLAGS) -o ./build/$@ $^  $(LIBS)

bmi: ./src/bme.c ./src/vm.c ./src/string_view.c ./src/label.c
	$(CC) $(CFLAGS) -o ./build/$@ $^  $(LIBS)

.PHONY:examples
examples:./examples/fib.bm ./examples/123.bm

# ./examples/fib.bm : %.ebasm
# 	./ebasm ./examples/fib.ebasm ./examples/fib.bm

# ./examples/123.bm : %.ebasm
# 	./ebasm ./examples/123.ebasm ./examples/123.bm