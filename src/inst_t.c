//
// Created by xm on 2021/8/15.
//
#include "inst_t.h"
#include "assert.h"
bool names_to_type(string_view inst_name,inst_t* out){
    for (inst_t type = (inst_t)0; type < AMOUNT_OF_INSTS;type +=1){
        if (sv_eq(cstr_as_string_view(inst_names(type)),inst_name)){
            *out = type;
            return true;
        }
    }
    return false;
}

const char* inst_names(inst_t type){
  switch (type) {
        case INST_NOP:      {return "nop";}
        case INST_PUSH:     {return "push";}
        case INST_ADDI:     {return "addi";}
        case INST_MULI:     {return "muli";}
        case INST_DIVI:     {return "divi";}
        case INST_SUBI:     {return "subi";}
        case INST_ADDF:     {return "addf";}
        case INST_MULF:     {return "mulf";}
        case INST_DIVF:     {return "divf";}
        case INST_SUBF:     {return "subf";}
        case INST_NOT:      {return "not";}
        case INST_JMP :     {return "jmp";}
        case INST_HALT:     {return "halt";}
        case INST_JMP_IF:   {return "jmp_if";}
        case INST_EQ:       {return "eq";}
        case INST_DROP:     {return "drop";}
        case INST_GEF:      {return "gef";}
        case INST_CALL:     {return "call";}
        case INST_SWAP:     {return "swap";}
        case INST_FFI:      {return "ffi";}
        case INST_DUP:      {return "dup";}
        case INST_ANDB:     {return "and";}
        case INST_ORB:      {return "or";}
        case INST_SHR:      {return "shr";}
        case INST_SHL:      {return "shl";}
        case INST_XORB:     {return "xor";}
        case INST_RET:      {return "ret";}
        case INST_NOTB:     {return "notb";}
        case INST_READ8:{return "read8";}
        case INST_READ16:{return "read16";}
        case INST_READ32:{return "read32";}
        case INST_READ64:{return "read64";}
        case INST_WRITE8:{return "write8";}
        case INST_WRITE16:{return "write16";}
        case INST_WRITE32:{return "write32";}
        case INST_WRITE64:{return "write64";}
        case AMOUNT_OF_INSTS:
        default:assert(0 && "inst_type_as_cstr:Unreachable");
  }
    return "";
}

bool inst_has_op(inst_t type){
  switch (type) {
        case INST_NOP:{return false;}
        case INST_ADDI:{return false;}
        case INST_SUBI:{return false;}
        case INST_MULI:{return false;}
        case INST_SUBF:{return false;}
        case INST_MULF:{return false;}
        case INST_DIVF:{return false;}
        case INST_HALT:{return false;}
        case INST_RET:{return false;}
        case INST_DROP:{return false;}
        case INST_ADDF:{return false;}
        case INST_DIVI:{return false;}
        case INST_EQ:{return false;}
        case INST_GEF:{return false;}
        case INST_NOT:{return false;}
        case INST_PUSH: {return true;}
        case INST_JMP:  {return true;}
        case INST_JMP_IF:{return true;}
        case INST_DUP:{return true;}
        case INST_SWAP:{return true;}
        case INST_CALL:{return true;}
        case INST_FFI:{return true;}
        case INST_ANDB:{return false;}
        case INST_ORB:{return false;}
        case INST_XORB:{return false;}
        case INST_SHL:{return false;}
        case INST_SHR:{return false;}
        case INST_NOTB:{return false;}
      case INST_READ8:{return false;}
      case INST_READ16:{return false;}
      case INST_READ32:{return false;}
      case INST_READ64:{return false;}
      case INST_WRITE8:{return false;}
      case INST_WRITE16:{return false;}
      case INST_WRITE32:{return false;}
      case INST_WRITE64:{return false;}
      case AMOUNT_OF_INSTS:{return false;}
        default:{
          return false;
        }
  }
    return false;
}
