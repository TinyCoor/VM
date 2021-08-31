//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_UTILS_H_
#define VM_SRC_UTILS_H_
#include "string_view.h"
#include "vm.h"

int number_liter_as_word(string_view sv, Word* out);
void vm_dump_stack(FILE * stream,const vm* machine);

void vm_dump_mem(FILE* stream,const vm* machine);

Word u64_to_word(uint64_t);
Word i64_to_word(int64_t);
Word f64_to_word(double );
Word ptr_to_word(void* );

const char* format(char* format,...);

#endif //VM_SRC_UTILS_H_
