//
// Created by xm on 2021/8/15.
//

#include "utils.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>


Word number_liter_as_word(string_view sv){
  assert(sv.count <1024);
  char cstr[sv.count +1];
  char* endptr = NULL;

  memcpy(cstr,sv.data,sv.count);
  cstr[sv.count]='\0';

  Word result={0};

  result.as_u64 = strtoull(cstr,&endptr,10);
  if (endptr - cstr !=sv.count){
    result.as_f64 = strtod(cstr,&endptr);
    if (endptr -cstr != sv.count){
      fprintf(stderr,"ERROR: '%s' is not number literal\n",cstr);
      exit(-1);
    }
  }
  return result;
}

void vm_dump_stack(FILE * stream,const vm* machine){
  printf("Stack:\n");
  if (machine->stack_size >0) {
    for (size_t i = 0; i < machine->stack_size; ++i) {
      fprintf(stream,"u64:%llu i64: %lld  f64: %lf  ptr: %p\n",
              machine->stack[i].as_u64,
              machine->stack[i].as_i64,
              machine->stack[i].as_f64,
              machine->stack[i].as_ptr);
    }
  } else{
    fprintf(stream,"[empty]\n");
  }
}

int fmt(char *buf,const char* format,va_list args){
  char* format_str = buf;

  int flags = 0;

  int var_count = 0; //{} 的个数

  for (const char* p = format;*p !='\0';++p){

  }

}

static char buf[1024];
const char* format(char* format,...){
  va_list args;
  int n=0;
  va_start(args,format);
  n = fmt(buf,format,args);
  va_end(args);

}



