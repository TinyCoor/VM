//
// Created by xm on 2021/8/8.
//
#include "vm.h"
#include "file.h"
vm machine={0};

int main(int argc,char* argv[]){
  if (argc < 2){
    fprintf(stderr,"Usage:./disassmble <input.vm>");
    fprintf(stderr,"ERROR:no input");
    exit(-1);
  }
  load_program_from_file(&machine,argv[1]);

  for (int i = 0; i < machine.program_size; ++i) {
    printf("%s",inst_name(machine.program[i].type));
    if (inst_has_op[machine.program[i].type]){
      printf(" %ld",machine.program[i].operand.as_i64);
    }
    printf("\n");
  }

  return 0;
}
