//
// Created by xm on 2021/7/20.
//

#ifndef VM__INSTRUCTION_H
#define VM__INSTRUCTION_H

#include "string_view.h"
#include "label.h"
#include "inst_t.h"
#include "error_t.h"
#include "native_call.h"
#include "assmbler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define VM_STACK_CAPACITY 1024
#define PROGRAM_CAPACITY 1024
#define FFI_TABLE_CAPACITY 1024

typedef uint64_t inst_addr;

typedef struct vm{
  //stack
  Word stack[VM_STACK_CAPACITY];
  uint64_t stack_size;

  //program
  inst program[PROGRAM_CAPACITY];
  uint64_t program_size;
  inst_addr ip;

  //native_func
  native_func ffi[FFI_TABLE_CAPACITY];
  size_t ffi_size;

  int halt;
} vm;

err_t vm_execute_inst(vm *machine);
err_t vm_execute_program(vm *machine, int limit);
#endif //VM__INSTRUCTION_H
