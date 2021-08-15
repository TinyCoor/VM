//
// Created by xm on 2021/8/12.
//

#include "label.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int label_table_find_addr(const label_table* lt,string_view name){
  for (size_t i = 0; i <lt->label_size ; ++i) {
    if (sv_eq(lt->labels[i].name,name)){
      return lt->labels[i].addr;
    }
  }
  fprintf(stderr,"ERROR: label %.*s does not exist\n",
          name.count,name.data);
  exit(-1);
  return -1;
}

void label_table_push_label(label_table* lt, string_view name,inst_addr addr){
  assert(lt->label_size <LABEL_CAPACITY);
  lt->labels[lt->label_size++] = (label_t){name,addr};
}

void label_table_push_unresolved_label(label_table* lt,
                                       string_view name,
                                       inst_addr addr
                                       ){
  assert(lt->unresolved_size <UNRESOLVED_LABEL_CAPACITY);
  lt->unresolved_labels[lt->unresolved_size++] = (unresolved_label){
    name,
    addr
  };
}


void print_label_table(label_table* lt){
  printf("Labels:\n");
  for (int i = 0; i <lt->label_size ; ++i) {
    printf("%.*s ->%ld\n",lt->labels[i].name.count,lt->labels[i].name.data,lt->labels[i].addr);
  }

  printf("Unresolved Labels:\n");
  for (int i = 0; i <lt->unresolved_size ; ++i) {
    printf("%ld ->%.*s\n",lt->unresolved_labels[i].addr,lt->unresolved_labels[i].name.count,lt->unresolved_labels[i].name.data);
  }
}