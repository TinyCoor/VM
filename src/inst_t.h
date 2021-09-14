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
} Word;

static_assert(sizeof(Word) == 8, "Only support x64 Arch");

//TODO impl more inst 
// converter needed  interger -> float float->integer
typedef enum {
    INST_NOP = 0,
    INST_PUSH,
    INST_ADDI,
    INST_SUBI,
    INST_MULI,
    INST_DIVI,
    INST_MODI,
    INST_ADDF,
    INST_SUBF,
    INST_MULF,
    INST_DIVF,
    INST_JMP,
    INST_JMP_IF,
    INST_DUP,
    INST_NOT,
    INST_HALT,
    INST_SWAP,
    INST_EQI,
    INST_GEI,
    INST_GTI,
    INST_LTI,
    INST_LEI,
    INST_NEI,
    INST_EQF,
    INST_GEF,
    INST_GTF,
    INST_LTF,
    INST_LEF,
    INST_NEF,
    INST_RET,
    INST_DROP,
    INST_CALL,
    INST_FFI,
    INST_ANDB,
    INST_ORB,
    INST_XORB,
    INST_SHL,
    INST_SHR,
    INST_NOTB,
    INST_READ8,
    INST_READ16,
    INST_READ32,
    INST_READ64,
    INST_WRITE8,
    INST_WRITE16,
    INST_WRITE32,
    INST_WRITE64,

    INST_I2F,
    INST_U2F,
    INST_F2I,
    INST_F2U,
    AMOUNT_OF_INSTS,
} inst_t;

typedef struct {
    inst_t type;
    Word operand;
} inst;


const char *inst_names(inst_t type);

bool names_to_type(string_view inst_name, inst_t *type);

bool inst_has_op(inst_t type);

#endif //VM_SRC_INST_T_H_
