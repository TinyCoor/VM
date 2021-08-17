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
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>

#define VM_STACK_CAPACITY 1024
#define PROGRAM_CAPACITY 1024

#define ARRAY_SIZE(xs) (sizeof(xs)/sizeof((xs)[0]))
#define EXEC_LIMIT 1024
#define MAKE_INST_PUSH(value) ((inst){.type = INST_PUSH,.operand=(value)})
#define MAKE_INST_PLUS ((inst){.type = INST_PLUS })
#define MAKE_INST_MINUS ((inst){.type = INST_MINUS })
#define MAKE_INST_MULT ((inst){.type = INST_MULT })
#define MAKE_INST_DIV ((inst){.type = INST_DIV })
#define MAKE_INST_NOP ((inst){.type = INST_NOP})
#define MAKE_INST_JMP(addr) ((inst){.type = INST_JMP,.operand=(addr)})
#define MAKE_INST_DUP(addr) ((inst){.type = INST_DUP,.operand=(addr)})
#define MAKE_INST_HALT(addr) ((inst){.type = INST_HALT,.operand=(addr)})

typedef uint64_t inst_addr;

typedef union {
  uint64_t as_u64;
  int64_t as_i64;
  double as_f64;
  void *as_ptr;
}Word;

static_assert(sizeof(Word)==8,"Only support x64 Arch");

typedef struct {
  inst_t type;
  Word operand;
} inst;

typedef struct {
  Word stack[VM_STACK_CAPACITY];
  uint64_t stack_size;

  inst program[PROGRAM_CAPACITY];
  uint64_t program_size;
  inst_addr ip;
  int halt;
} vm;


void push_inst(vm *machine, inst ins);

err_t vm_execute_inst(vm *machine);
err_t vm_execute_program(vm *machine, int limit);
void translate_source(string_view src,
                      vm *machine,
                      label_table *lt);
#endif //VM__INSTRUCTION_H
