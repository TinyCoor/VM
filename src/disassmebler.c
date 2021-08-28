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
    printf("%ld : %s",(long int)i,inst_names(machine.program[i].type));
    if (inst_has_op(machine.program[i].type)){
      printf(" %lld",machine.program[i].operand.as_u64);
    }
    printf("\n");
  }

  return 0;
}
