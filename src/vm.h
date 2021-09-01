//
// Created by xm on 2021/7/20.
//

#ifndef VM__INSTRUCTION_H
#define VM__INSTRUCTION_H

#include "string_view.h"
#include "context.h"
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
#define FFI_TABLE_CAPACITY 1024


typedef uint64_t inst_addr;
typedef uint64_t mem_addr;

typedef struct vm{
  //stack
  Word stack[VM_STACK_CAPACITY];
  uint64_t stack_size;

inst program[PROGRAM_CAPACITY];
uint64_t program_size;

uint8_t memory[MAX_STATIC_MEM];
size_t memory_size;
size_t memory_capacity;
  //program
  inst_addr ip;

  //native_func
  native_func ffi[FFI_TABLE_CAPACITY];
  size_t ffi_size;

  uint8_t allocator[MAX_STATIC_MEM];
  size_t allocator_size;
  bool halt;
} vm;



err_t vm_execute_inst(vm *machine);
err_t vm_execute_program(vm *machine, int limit);
#endif //VM__INSTRUCTION_H
