//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_INST_T_H_
#define VM_SRC_INST_T_H_

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
  INST_HALT,
  INST_SWAP,
  INST_PRINT_DEBUG,
  AMOUNT_OF_INSTS,
} inst_t;

const char* inst_names(inst_t type);

const char* inst_type_as_cstr(inst_t inst_type);
int inst_has_op(inst_t type);
#endif //VM_SRC_INST_T_H_
