//
// Created by xm on 2021/8/7.
//
#include "string_view.h"
#include<ctype.h>

string_view sv_chop_by_delim(string_view* sv,char delim){
  size_t i =0;
  while (i<sv->count && sv->data[i] != delim){
    i+=1;
  }
  string_view result = {i,sv->data};
  if (i < sv->count){
    sv->count -= i+1;
    sv->data += i+1;
  } else{
    sv->count -= i;
    sv->data += i;
  }
  return result;
}

string_view sv_trim_left(string_view sv){
  size_t i =0;
  while (i<sv.count && isspace(sv.data[i])){
    i+=1;
  }
  return (string_view){sv.count -i,sv.data +i};
}

string_view sv_trim_right(string_view sv){
  size_t i =0;
  while (i<sv.count && isspace(sv.data[sv.count -1 -i])){
    i+=1;
  }
  return (string_view){sv.count -i,sv.data +i};
}

string_view sv_trim(string_view sv){
  return sv_trim_left(sv_trim_right(sv));
}

string_view cstr_as_string_view(const char* cstr){
  return (string_view){strlen(cstr),cstr};
}

int sv_to_int(string_view view){
  int result = 0;
  size_t i =0;
  for (size_t i =0; i<view.count&& isdigit(view.data[i]);++i ){
    result = result*10+*view.data- '0';
  }
  return result;
}

bool sv_eq(string_view lhs,string_view rhs){
  if (lhs.count != rhs.count ){
    return false;
  } else{
    return memcmp(lhs.data,rhs.data,rhs.count) ==0;
  }
}
