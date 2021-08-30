//
// Created by xm on 2021/8/15.
//

#include "error_t.h"
#include <assert.h>
const char* err_as_cstr(err_t trap){
  switch (trap) {
    case ERR_OK:{return "ERR_OK";}
    case ERR_STACK_OVERFLOW:{return "ERR_STACK_OVERFLOW";}
    case ERR_STACK_UNDERFLOW:{return "ERR_STACK_UNDERFLOW";}
    case ERR_ILLEGAL_INST:{return "ERR_ILLEGAL_INST";}
    case ERR_DIV_BY_ZERO:{return "ERR_DIV_BY_ZERO";}
    case ERR_ILLEGAL_OPERAND:{return "ERR_ILLEGAL_OPERAND";}
    case ERR_ILLEGAL_INST_ACCESS:{return "ERR_ILLEGAL_INST_ACCESS";}
    case ERR_ILLEGAL_MEM_ACCESS:{return "ERR_ILLEGAL_MEM_ACCESS";}
    default:{assert(0 &&"trap_as_cstr: Unreachable");}
  }
  return "";

}
