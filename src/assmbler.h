//
// Created by xm on 2021/8/7.
//

#ifndef VM__PARSER_H
#define VM__PARSER_H
#include "label.h"
#include "string_view.h"
typedef struct vm vm;

void assmble_source(const char* ,
                      vm *machine,
                      label_table *lt);

#endif //VM__PARSER_H
