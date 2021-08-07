#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define VM_STACK_CAPACITY 1024
#define PROGRAM_CAPACITY 1024
#define ARRAY_SIZE(xs) (sizeof(xs)/sizeof((xs)[0]))

typedef int64_t word;

typedef enum {
  ERR_OK,
  ERR_STACK_OVERFLOW,
  ERR_STACK_UNDERFLOW,
  ERR_ILLEGAL_INST,
  ERR_DIV_BY_ZERO,
}err_t;

const char* err_as_cstr(err_t trap){
  switch (trap) {
    case ERR_OK:
      return "ERR_OK";
    case ERR_STACK_OVERFLOW:
      return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
      return "TRAP_STACK_UNDERFLOW";
    case ERR_ILLEGAL_INST:
      return "ERR_ILLEGAL_INST";
    case ERR_DIV_BY_ZERO:
      return "ERR_DIV_BY_ZERO";
    default:
      assert(0 &&"trap_as_cstr: Unreachable");
  }
}


typedef enum{
  INST_PUSH,
  INST_PLUS,
  INST_MINUS,
  INST_MULT,
  INST_DIV,
  INST_JMP,
  INST_HALT,
}inst_t;

const char* inst_type_as_cstr(inst_t inst_type){
  switch (inst_type) {
    case INST_PUSH:{return "INST_PUSH";}
    case INST_PLUS:{return "INST_PLUS";}
    case INST_MULT:{return "INST_MULT";}
    case INST_DIV:{return "INST_DIV";}
    case INST_MINUS:{return "INST_MINUS";}
    case INST_JMP:{return "INST_JMP";}
    case INST_HALT:{return "INST_HALT";}
    default:assert(0&& "inst_type_as_cstr:Unreachable");
  }
}

typedef struct {
  inst_t type;
  word operand;
} inst;


typedef struct {
  word stack[VM_STACK_CAPACITY];
  size_t stack_size;

  inst program[PROGRAM_CAPACITY];
  size_t program_size;

  word ip;
  int halt;
} vm;

void push_inst(vm* machine,inst ins){
  assert(machine->program_size <PROGRAM_CAPACITY);
  machine->program[machine->program_size++] = ins;
}

err_t vm_execute_inst(vm* machine,inst inst){
  switch (inst.type) {
    case INST_PUSH: {
      if(machine->stack_size >= VM_STACK_CAPACITY){
        return ERR_STACK_OVERFLOW;
      }
      machine->ip +=1;
      machine->stack[machine->stack_size++]=inst.operand;
    }break;
    case INST_PLUS: {
      if (machine->stack_size <2){
        return ERR_STACK_UNDERFLOW;
      }
      machine->stack[machine->stack_size-2] +=machine->stack[machine->stack_size-1];
      machine->stack_size -=1;
      machine->ip+=1;
    }break;
    case INST_MINUS:{
      if (machine->stack_size <2){
        return ERR_STACK_UNDERFLOW;
      }
      machine->stack[machine->stack_size-2] -=machine->stack[machine->stack_size-1];
      machine->stack_size -=1;
    }break;
    case INST_MULT: {
      if (machine->stack_size <2){
        return ERR_STACK_UNDERFLOW;
      }
      machine->stack[machine->stack_size-2] *=machine->stack[machine->stack_size-1];
      machine->stack_size -=1;
      machine->ip+=1;
    }break;
    case INST_DIV:  {
      if (machine->stack_size <2){
        return ERR_STACK_UNDERFLOW;
      }
      if(machine->stack[machine->stack_size-1] ==0){
        return ERR_DIV_BY_ZERO;
      }
      machine->stack[machine->stack_size-2] /=machine->stack[machine->stack_size-1];
      machine->stack_size -=1;
      machine->ip+=1;
    }break;
    case INST_JMP:{
      if (inst.operand < 0 || inst.operand > ){

      }
    }break;
    case INST_HALT:{

    }break;
  default:
    return ERR_ILLEGAL_INST;
  }
  return ERR_OK;
}

void vm_dump_stack(FILE * stream,const vm* machine){
  printf("Stack:\n");
  if (machine->stack_size >0) {
    for (size_t i = 0; i < machine->stack_size; ++i) {
      fprintf(stream,"%lld\n", machine->stack[i]);
    }
  } else{
    fprintf(stream,"[empty]\n");
  }
}

#define MAKE_INST_PUSH(value) ((inst){.type = INST_PUSH,.operand=(value)})
#define MAKE_INST_PLUS ((inst){.type = INST_PLUS })
#define MAKE_INST_MINUS ((inst){.type = INST_MINUS })
#define MAKE_INST_MULT ((inst){.type = INST_MULT })
#define MAKE_INST_DIV ((inst){.type = INST_DIV })
#define MAKE_INST_JMP(addr) ((inst){.type = INST_JMP,.operand=(addr)})
#define MAKE_INST_HALT(addr) ((inst){.type = INST_HALT,.operand=(addr)})

vm machine = {0};
inst program[]={
    MAKE_INST_PUSH(68),
    MAKE_INST_PUSH(89),
    MAKE_INST_PLUS,
    MAKE_INST_PUSH(78),
    MAKE_INST_MINUS,
    MAKE_INST_PUSH(2),
    MAKE_INST_MULT,
    MAKE_INST_PUSH(0),
    MAKE_INST_DIV,
};

void load_program_from_memory(vm* machine,inst* program,size_t program_size){
  assert(program_size <PROGRAM_CAPACITY);
  memcpy(machine->program,program,program_size);
  machine->program_size=program_size;
}

void load_program_from_file(word* ptr,size_t program_size){

}

int main(int argc,char* argv[]) {
  load_program_from_memory(&machine,program,ARRAY_SIZE(program));
  vm_dump_stack(stdout,&machine);
  while (!machine.halt){
    for (size_t i=0;i <ARRAY_SIZE(program);++i){
      printf("%s\n",inst_type_as_cstr(program[machine.ip].type));
      err_t trap =vm_execute_inst(&machine,program[machine.ip]);
      vm_dump_stack(stdout,&machine);
      if (trap !=ERR_OK){
        fprintf(stderr,"ERROR: %s\n",err_as_cstr(trap));
        exit(-1);
      }
    }
  }
}
