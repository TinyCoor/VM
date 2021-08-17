//
// Created by xm on 2021/8/15.
//
#include "inst_t.h"
#include "assert.h"
const char* inst_type_as_cstr(inst_t inst_type){
  switch (inst_type) {
    case INST_NOP:{return "INST_NOP";}
    case INST_PUSH:{return "INST_PUSH";}
    case INST_ADDI:{return "INST_ADDI";}
    case INST_MULI:{return "INST_MULI";}
    case INST_DIVI:{return "INST_DIVI";}
    case INST_SUBI:{return "INST_SUBI";}
    case INST_ADDF:{return "INST_ADDF";}
    case INST_MULF:{return "INST_MULF";}
    case INST_DIVF:{return "INST_DIVF";}
    case INST_SUBF:{return "INST_SUBF";}
    case INST_JMP :{return "INST_JMP";}
    case INST_HALT:{return "INST_HALT";}
    case INST_JMP_IF:{return "INST_JMP_IF";}
    case INST_EQ:{return "INST_EQ";}
    case INST_PRINT_DEBUG:{return "INST_PRINT_DEBUG";}
    case INST_DUP:{return "INST_DUP";}
    case INST_SWAP:{return "INST_SWAP";}
    case AMOUNT_OF_INSTS:
    default:assert(0&& "inst_type_as_cstr:Unreachable");
  }
}

const char* inst_names(inst_t type){
  switch (type) {
    case INST_NOP:    {return "nop";}
    case INST_PUSH:   {return "push";}
    case INST_ADDI:   {return "addi";}
    case INST_MULI:   {return "muli";}
    case INST_DIVI:   {return "divi";}
    case INST_SUBI:   {return "subi";}
    case INST_ADDF:   {return "addf";}
    case INST_MULF:   {return "mulf";}
    case INST_DIVF:   {return "divf";}
    case INST_SUBF:   {return "subf";}
    case INST_JMP :   {return "jmp";}
    case INST_HALT:   {return "halt";}
    case INST_JMP_IF: {return "jmp_if";}
    case INST_EQ:     {return "eq";}
    case INST_SWAP: {return "swap";}
    case INST_PRINT_DEBUG:{return "print_debug";}
    case INST_DUP:      {return "dup";}
    case AMOUNT_OF_INSTS:
    default:assert(0&& "inst_type_as_cstr:Unreachable");
  }
}


int inst_has_op(inst_t type){
  switch (type) {
    case INST_PUSH: {return 1;}
    case INST_JMP:  {return 1;}
    case INST_JMP_IF:{return 1;}
    case INST_DUP:{return 1;}
    default:{
      return 0;
    }
  }
}