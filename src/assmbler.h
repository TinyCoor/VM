//
// Created by xm on 2021/8/7.
//

#ifndef VM__PARSER_H
#define VM__PARSER_H
#include "label.h"
#include "string_view.h"
typedef struct vm vm;

void assmble_source(string_view ,
                      vm *machine,
                      label_table *lt,
                      size_t level);

#endif //VM__PARSER_H
