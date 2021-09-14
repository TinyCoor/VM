//
// Created by xm on 2021/8/15.
//

#ifndef VM_SRC_FILE_H_
#define VM_SRC_FILE_H_
#include "string_view.h"
#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

#if defined(__GNU__) || defined(__clang__)
#define PACKED __attribute__((packed))
#elif defined(_MSC_VER)
#define PACKED (__Declaration__) __pragma(pack(push ,1)) __Declaration__ __pragma(pack(pop))
#else
#define PACKED
#endif

#define MAGIC 0x4d42
#define VERSION 1


typedef struct {
    uint16_t magic;
    uint16_t version;
    uint64_t program_size;
    uint64_t memory_size;
    uint64_t memory_capacity;
}PACKED file_meta_data;

void save_program_to_file(context * ctx,
                          const char *file_path);

void load_program_from_file(vm*, const char *file_name);

string_view slurp_file(string_view);

string_view ctx_slurp_file(context * lt,string_view file_path);

#endif //VM_SRC_FILE_H_
