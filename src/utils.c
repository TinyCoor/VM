//
// Created by xm on 2021/8/15.
//

#include "utils.h"
#include <assert.h>
#include <string.h>


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
    if (endptr -cstr !=sv.count){
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
      fprintf(stream,"u64:%lu f64: %lf i64: %ld  ptr: %x\n",
              machine->stack[i].as_u64,
              machine->stack[i].as_f64,
              machine->stack[i].as_i64,
              machine->stack[i].as_ptr);
    }
  } else{
    fprintf(stream,"[empty]\n");
  }
}

