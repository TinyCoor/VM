//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_UTILS_H_
#define VM_SRC_UTILS_H_
#include "string_view.h"
#include "vm.h"
Word number_liter_as_word(string_view sv);
void vm_dump_stack(FILE * stream,const vm* machine);

#endif //VM_SRC_UTILS_H_
