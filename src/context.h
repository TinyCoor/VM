//
// Created by xm on 2021/8/12.
//

#ifndef VM_SRC_LABEL_H
#define VM_SRC_LABEL_H

#include "string_view.h"
#include "inst_t.h"

#define LABEL_CAPACITY 1024
#define UNRESOLVED_LABEL_CAPACITY 1024
#define MAX_MEM_SIZE (1024 * 1024)
#define PROGRAM_CAPACITY 1024
#define MAX_STATIC_MEM (640 * 1000)

typedef uint64_t inst_addr;

typedef struct {
    string_view name;
    Word value;
} binding;

typedef struct {
    string_view name;
    inst_addr addr;
} deferred_binding;

typedef struct {
    binding labels[LABEL_CAPACITY];
    size_t label_size;

    deferred_binding deferred_labels[UNRESOLVED_LABEL_CAPACITY];
    size_t deferred_size;

    inst program[PROGRAM_CAPACITY];
    uint64_t program_size;

    uint8_t memory[MAX_STATIC_MEM];
    size_t memory_size;
    size_t memory_capacity;

    char arena[MAX_MEM_SIZE];
    size_t arena_size;
} context;


void *ctx_alloc_memory(context *lt, size_t size);

bool ctx_resolve_label(const context *,
                       string_view name,
                       Word *);

//TODO label should refered to Word instead of inst_addr
int ctx_bind_label(context *,
                   string_view,
                   Word);

Word ctx_push_string_to_memory(context* , string_view);

void ctx_push_deferred_label(context *,
                             string_view,
                             inst_addr addr);


#endif //VM_SRC_LABEL_H
