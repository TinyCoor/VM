//
// Created by xm on 2021/8/8.
//
#include "vm.h"
vm machine{0};

int main(int argc,char* argv[]){
  if (argc < 2){
    fprintf(stderr,"Usage:./disassmble <input.vm>");
    fprintf(stderr,"ERROR:no input");
    exit(-1);
  }
  load_program_from_file(&machine,argv[1]);
  for (int i = 0; i < machine.program_size; ++i) {
    switch (machine.program[i].type) {
    case INST_NOP:{
      printf("nop\n");
    }break;
    case INST_PUSH:{
      printf("push %d\n",machine.program[i].operand)
    }break;
    case INST_PLUS:{
      printf("plus\n");
    }break;
    case INST_MINUS:{
      printf("minus\n");
    }break;
    case INST_MULT:{
      printf("mul\n");
    }break;
    case INST_DIV:{
      printf("div\n");
    }break;
    case INST_JMP:{
      printf("jmp %ld\n",machine.program[i].operand)
    }break;
    case INST_JMP_IF:{
      printf("jmp_if %ld\n",machine.program[i].operand)
    }break;
    case INST_EQ:{
      printf("jmp %ld\n",machine.program[i].operand)
    }break;
    case INST_DUP:{
      printf("dup %ld\n",machine.program[i].operand)
    }break;
    case INST_HALT:{

    }break;
    case INST_PRINT_DEBUG{

    }break;
    }
  }

  return 0;
}
