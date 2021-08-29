//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_FILE_H_
#define VM_SRC_FILE_H_
#include "string_view.h"
#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

void save_program_to_file(inst *program,
                          size_t program_size,
                          const char *file_path);

void load_program_from_file(vm *machine, const char *file_name);

string_view slurp_file(string_view);

string_view label_table_slurp_file(label_table* lt,string_view file_path);

void load_program_from_memory(vm *machine, inst *program,
                              size_t program_size);

#endif //VM_SRC_FILE_H_
