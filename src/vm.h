//
// Created by xm on 2021/7/20.
//

#ifndef VM__INSTRUCTION_H
#define VM__INSTRUCTION_H

#include "string_view.h"
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

typedef int64_t word;
typedef enum {
  ERR_OK,
  ERR_STACK_OVERFLOW,
  ERR_STACK_UNDERFLOW,
  ERR_ILLEGAL_INST,
  ERR_ILLEGAL_OPERAND,
  ERR_DIV_BY_ZERO,
  ERR_ILLEGAL_INST_ACCESS

}err_t;
typedef enum{
  INST_NOP,
  INST_PUSH,
  INST_PLUS,
  INST_MINUS,
  INST_MULT,
  INST_DIV,
  INST_JMP,
  INST_JMP_IF,
  INST_EQ,
  INST_DUP,
  INST_HALT,
  INST_PRINT_DEBUG,
}inst_t;
typedef struct {
  inst_t type;
  word operand;
} inst;
typedef struct {
  word stack[VM_STACK_CAPACITY];
  word stack_size;

  inst program[PROGRAM_CAPACITY];
  word program_size;
  word ip;

  int halt;
} vm;

const char* err_as_cstr(err_t trap);
const char* inst_type_as_cstr(inst_t inst_type);
void push_inst(vm* machine,inst ins);
err_t vm_execute_inst(vm* machine);
err_t vm_execute_program(vm* machine,int limit);
err_t get_stack_frame(vm* machine);
void vm_dump_stack(FILE * stream,const vm* machine);
void load_program_from_memory(vm* machine,inst* program,size_t program_size);
void save_program_to_file(inst* program,
                          size_t program_size,
                          const char* file_path);
void load_program_from_file(vm* machine, const char* file_name);
inst translate_line(string_view line);
size_t translate_src(string_view src,inst* program,size_t program_capacity);
string_view slurp_file(const char* file_name);
#endif //VM__INSTRUCTION_H
