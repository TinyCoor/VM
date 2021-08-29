//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_UTILS_H_
#define VM_SRC_UTILS_H_
#include "string_view.h"
#include "vm.h"
int number_liter_as_word(string_view sv, Word* out);
void vm_dump_stack(FILE * stream,const vm* machine);

void err_to_std(FILE* stream,string_view fmt,string_view,string_view);

const char* format(char* format,...);

#endif //VM_SRC_UTILS_H_
