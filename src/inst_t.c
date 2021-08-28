//
// Created by xm on 2021/8/15.
//
#include "inst_t.h"
#include "assert.h"
#include "string_view.h"
int names_to_type(string_view inst_name,inst_t* out){
    for (inst_t type = (inst_t)0; type < AMOUNT_OF_INSTS;type +=1){
        if (sv_eq(cstr_as_string_view(inst_names(type)),inst_name)){
            *out = type;
            return 1;
        }
    }
    return 0;
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
    case INST_NOT:    {return "not";}
    case INST_JMP :   {return "jmp";}
    case INST_HALT:   {return "halt";}
    case INST_JMP_IF: {return "jmp_if";}
    case INST_EQ:     {return "eq";}
    case INST_DROP:   {return "drop";}
    case INST_GEF:    {return "gef";}
    case INST_CALL:   {return "call";}
    case INST_SWAP:   {return "swap";}
    case INST_FFI:    {return "ffi";}
    case INST_DUP:      {return "dup";}
    case INST_URMOM: {return "urmom";}
    case INST_RET:{return "ret";}
    case AMOUNT_OF_INSTS:
    default:assert(0&& "inst_type_as_cstr:Unreachable");
  }
    return "";
}

int inst_has_op(inst_t type){
  switch (type) {
      case INST_NOP:{return 0;}
      case INST_ADDI:{return 0;}
      case INST_SUBI:{return 0;}
      case INST_MULI:{return 0;}
      case INST_SUBF:{return 0;}
      case INST_MULF:{return 0;}
      case INST_DIVF:{return 0;}
      case INST_HALT:{return 0;}
      case INST_RET:{return 0;}
      case AMOUNT_OF_INSTS:{return 0;}
      case INST_DROP:{return 0;}
      case INST_ADDF:{return 0;}
      case INST_DIVI:{return 0;}
      case INST_EQ:{return 0;}
      case INST_GEF:{return 0;}
      case INST_NOT:{return 0;}
    case INST_PUSH: {return 1;}
    case INST_JMP:  {return 1;}
    case INST_JMP_IF:{return 1;}
    case INST_DUP:{return 1;}
    case INST_SWAP:{return 1;}
    case INST_CALL:{return 1;}
    case INST_FFI:{return 1;}
    case INST_URMOM:{return 1;}
    default:{
      return 0;
    }
  }
    return 0;
}
