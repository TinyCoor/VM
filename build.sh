#!/usr/bin/env bash

set -xe

CC=${CC:=/usr/bin/cc}
CFLAGS = "-Wall -Wextra -Wswitch-enum -fno-strict-aliasing -pedantic -Wmissing-prototypes  -ggdb -std=c11 -O0 "
LIB_SRC ="./src/inst.c ./src/vm.c ./src/label.c ./src/string_view.c
./src/error_t.c ./src/file.c ./src/native_call.c ./src/utils.c"
LIBS =

$CC $CFLAGS -o basm ./src/basm.c ${LIB_SRC}   $LIBS
$CC $CFLAGS -o bme ./src/bmi.c ${LIB_SRC} $LIBS
$CC $CFLAGS -o debasm ./src/disassmebler.c ${LIB_SRC}  $LIBS

for example in `find examples/ -name \*.bm | sed "s/\.vbm//"`; do
    ./basm "$example.vm" "$example.vbm"
done