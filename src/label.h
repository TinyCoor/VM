//
// Created by xm on 2021/8/12.
//

#ifndef VM_SRC_LABEL_H
#define VM_SRC_LABEL_H
#include "string_view.h"
#include "inst_t.h"

#define LABEL_CAPACITY 1024
#define UNRESOLVED_LABEL_CAPACITY 1024
typedef uint64_t inst_addr;
typedef struct {
  string_view name;
  Word word;
}label_t;

typedef struct {
  string_view name;
  inst_addr addr;
}deferred_label;

typedef struct {
  label_t labels[LABEL_CAPACITY];
  size_t label_size;
  deferred_label deferred_labels[UNRESOLVED_LABEL_CAPACITY];
  size_t deferred_size;
}label_table;

int label_table_resolve_label(const label_table*,
                     string_view name,
                     Word* );

//TODO label should refered to Word instead of inst_addr
int label_table_bind_label(label_table*,
                      string_view ,
                      Word);

void label_table_push_deferred_label(label_table*,
                                       string_view,
                                       inst_addr addr);

void print_label_table(label_table* lt);

bool is_contains_deffered_label(label_table*);

#endif //VM_SRC_LABEL_H
