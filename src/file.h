//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_FILE_H_
#define VM_SRC_FILE_H_
#include "string_view.h"
#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

//typedef struct {
//
//}exec_file;

void save_program_to_file(context * ctx,
                          const char *file_path);

void load_program_from_file(vm *machine, const char *file_name);

string_view slurp_file(string_view);

string_view ctx_slurp_file(context * lt,string_view file_path);

void load_program_from_memory(vm *machine, inst *program,
                              size_t program_size);

#endif //VM_SRC_FILE_H_
