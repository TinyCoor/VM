//
// Created by xm on 2021/8/12.
//

#ifndef VM_SRC_LABEL_H
#define VM_SRC_LABEL_H
#include "string_view.h"

#define LABEL_CAPACITY 1024
#define UNRESOLVED_LABEL_CAPACITY 1024
typedef uint64_t inst_addr;
typedef struct {
  string_view name;
  inst_addr addr;
}label_t;

typedef struct {
  string_view name;
  inst_addr addr;
}unresolved_label;

typedef struct {
  label_t labels[LABEL_CAPACITY];
  size_t label_size;
  unresolved_label unresolved_labels[UNRESOLVED_LABEL_CAPACITY];
  size_t unresolved_size;
}label_table;

int label_table_find_addr(const label_table*,
                     string_view name);

void label_table_push_label(label_table*,
                      string_view ,
                      inst_addr addr);

void label_table_push_unresolved_label(label_table*,
                                       string_view,
                                       inst_addr addr);

void print_label_table(label_table* lt);

bool is_contains_unresolved_label(label_table*);



#endif //VM_SRC_LABEL_H
