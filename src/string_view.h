//
// Created by xm on 2021/8/7.
//

#ifndef VM__STRING_VIEW_H
#define VM__STRING_VIEW_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
  size_t count;
  const char *data;
}string_view;

#define SV_FORMAT(sv) (int)sv.count,sv.data

string_view cstr_as_string_view(const char* cstr);
string_view sv_chop_by_delim(string_view* sv,char c);
string_view sv_trim(string_view sv);
int find_last_of(string_view,char delim);
int sv_to_int(string_view view);
bool sv_eq(string_view lhs,string_view rhs);

#endif //VM__STRING_VIEW_H
