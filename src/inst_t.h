//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_INST_T_H_
#define VM_SRC_INST_T_H_
#include "string_view.h"
#include <stdint.h>
#include <assert.h>

typedef union {
  uint64_t as_u64;
  int64_t as_i64;
  double as_f64;
  void *as_ptr;
}Word;

static_assert(sizeof(Word)==8,"Only support x64 Arch");

//TODO impl more inst 
// converter needed  interger -> float float->integer
typedef enum {
  INST_NOP = 0,
  INST_PUSH,
  INST_ADDI,
  INST_SUBI,
  INST_MULI,
  INST_DIVI,
  INST_ADDF,
  INST_SUBF,
  INST_MULF,
  INST_DIVF,
  INST_JMP,
  INST_JMP_IF,
  INST_EQ,
  INST_DUP,
  INST_NOT,
  INST_HALT,
  INST_SWAP,
  INST_GEF,
  INST_RET,
  INST_DROP,
  INST_CALL,
  INST_FFI,
  AMOUNT_OF_INSTS,
} inst_t;

typedef struct {
  inst_t type;
  Word operand;
} inst;


const char* inst_names(inst_t type);
const char* inst_type_as_cstr(inst_t inst_type);
int  names_to_type(string_view inst_name,inst_t* type);

int inst_has_op(inst_t type);
#endif //VM_SRC_INST_T_H_
