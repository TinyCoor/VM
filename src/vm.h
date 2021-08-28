//
// Created by xm on 2021/7/20.
//

#ifndef VM__INSTRUCTION_H
#define VM__INSTRUCTION_H

#include "string_view.h"
#include "label.h"
#include "inst_t.h"
#include "error_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define VM_STACK_CAPACITY 1024
#define PROGRAM_CAPACITY 1024

typedef uint64_t inst_addr;
typedef struct {
  Word stack[VM_STACK_CAPACITY];
  uint64_t stack_size;

  inst program[PROGRAM_CAPACITY];
  uint64_t program_size;
  inst_addr ip;
  int halt;
} vm;

err_t vm_execute_inst(vm *machine);
err_t vm_execute_program(vm *machine, int limit);
void translate_source(string_view src,
                      vm *machine,
                      label_table *lt);
#endif //VM__INSTRUCTION_H
