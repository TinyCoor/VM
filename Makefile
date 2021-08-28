CFLAGS= -Wall -Wextra -Wswitch-enum -std=c11 -O3 -pedantic
CC = gcc


EXAMPLES:=$(patsubst %.basm,%.bm ,$(willcard ./examples/*.basm)
#EXAMPLES:=find examples/ -name \*.basm | sed "s/\.basm/\.bm"
.PHONY : all

all:basm debasm bme

debasm : ./src/disassmebler.c ./src/vm.c  ./src/string_view.c ./src/label.c		
	$(CC) $(CFLAGS) -o ./build/$@  $^ $(LIBS)

basm: ./src/main.c ./src/vm.c ./src/string_view.c ./src/label.c
	$(CC) $(CFLAGS) -o ./build/$@ $^  $(LIBS)

bme: ./src/bme.c ./src/vm.c ./src/string_view.c ./src/label.c
	$(CC) $(CFLAGS) -o ./build/$@ $^  $(LIBS)

.PHONY:examples
examples:$(EXAMPLES)

%.bm : %.basm
    ./basm $< $@

# ./examples/fib.bm : %.ebasm
# 	./ebasm ./examples/fib.ebasm ./examples/fib.bm

# ./examples/123.bm : %.ebasm
# 	./ebasm ./examples/123.ebasm ./examples/123.bm