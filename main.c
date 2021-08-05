#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define VM_STACK_CAPACITY 1024

typedef int64_t word;

typedef enum {
  TRAP_OK,
  TRAP_STACK_OVERFLOW,
  TRAP_STACK_UNDERFLOW,
  TRAP_ILLEGAL_INST,
}trap_t;

typedef struct {
  word stack[VM_STACK_CAPACITY];
  size_t stack_size;
} vm;

typedef enum{
  INST_PUSH,
  INST_PLUS,
}inst_t;

typedef struct {
  inst_t type;
  word operand;
} inst;



inst inst_push(word operand){
  return (inst){
    .type = INST_PUSH,
    .operand= operand,
  };
}

inst inst_plus(word operand){
  return (inst){
    .type =INST_PLUS,
    .operand= operand,
  };
}

trap_t vm_execute_inst(vm* machine,inst inst){
  switch (inst.type) {
    case INST_PUSH:{
      if(machine->stack_size >= VM_STACK_CAPACITY){
        return TRAP_STACK_OVERFLOW;
      }
      machine->stack[machine->stack_size++]=inst.operand;
    }
    break;
    case INST_PLUS:{
      if (machine->stack_size <2){
        return TRAP_STACK_UNDERFLOW;
      }
      machine->stack[machine->stack_size-2] +=machine->stack[machine->stack_size-1];
      machine->stack_size -=1;
    }
    break;
  default:
    return TRAP_ILLEGAL_INST;
  }
  return TRAP_OK;
}
vm machine = {0};
void vm_dump(const vm* machine){
  printf("Stack:\n");
  if (machine->stack_size >0) {
    for (size_t i = 0; i < machine->stack_size; ++i) {
      printf("%d\n", machine->stack[i]);
    }
  } else{
    printf("Empty\n");
  }
}

int main(int argc,char* argv[]) {
  vm_dump(&machine);
  vm_execute_inst( &machine,inst_push(68));
  vm_dump(&machine);
 vm_execute_inst( &machine, inst_push(89));
  vm_dump(&machine);
}
