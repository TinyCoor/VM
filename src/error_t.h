//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_ERROR_T_H_
#define VM_SRC_ERROR_T_H_

typedef enum {
  ERR_OK,
  ERR_STACK_OVERFLOW,
  ERR_STACK_UNDERFLOW,
  ERR_ILLEGAL_INST,
  ERR_ILLEGAL_OPERAND,
  ERR_DIV_BY_ZERO,
  ERR_ILLEGAL_INST_ACCESS
} err_t;



const char *err_as_cstr(err_t trap);

#endif //VM_SRC_ERROR_T_H_
