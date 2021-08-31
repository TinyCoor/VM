//
// Created by xm on 2021/8/7.
//

#ifndef VM__PARSER_H
#define VM__PARSER_H
#include "context.h"
#include "string_view.h"
#include "vm.h"

void assmble_source(string_view ,
                    context*,
                    size_t);

#endif //VM__PARSER_H
