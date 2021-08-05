#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define VM_STACK_CAPACITY 1024
#define ARRAY_SIZE(xs) (sizeof(xs)/sizeof((xs)[0]))


typedef int64_t word;

typedef enum {
  TRAP_OK,
  TRAP_STACK_OVERFLOW,
  TRAP_STACK_UNDERFLOW,
  TRAP_ILLEGAL_INST,
}trap_t;

const char* trap_as_cstr(trap_t trap){
  switch (trap) {
    case TRAP_OK:
      return "TRAP_OK";
    case TRAP_STACK_OVERFLOW:
      return "TRAP_STACK_OVERFLOW";
    case TRAP_STACK_UNDERFLOW:
      return "TRAP_STACK_UNDERFLOW";
    case TRAP_ILLEGAL_INST:
      return "TRAP_ILLEGAL_INST";
    default:
      assert(0 &&"Unreachable");
  }
}

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



static inline inst inst_push(word operand){
  return (inst){
    .type = INST_PUSH,
    .operand= operand,
  };
}

static inline inst inst_plus(){
  return (inst){
    .type =INST_PLUS,
    .operand= 0,
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

void vm_dump(FILE * stream,const vm* machine){
  printf("Stack:\n");
  if (machine->stack_size >0) {
    for (size_t i = 0; i < machine->stack_size; ++i) {
      fprintf(stream,"%lld\n", machine->stack[i]);
    }
  } else{
    fprintf(stream,"[empty]\n");
  }
}

vm machine = {0};
#define MAKE_INST_PUSH(value) ((inst){.type = INST_PUSH,.operand=(value)})
#define MAKE_INST_PLUS ((inst){.type = INST_PLUS })

//static inst program[]={
//    inst_push(68),
//    inst_push(89),
//    inst_plus(),
//};
//#else
inst program[]={
    MAKE_INST_PUSH(68),
    MAKE_INST_PUSH(89),
    MAKE_INST_PLUS,
};
//#endif

int main(int argc,char* argv[]) {
  vm_dump(stdout,&machine);
  for (size_t i=0;i <ARRAY_SIZE(program);++i){
    trap_t trap =vm_execute_inst(&machine,program[i]);
    if (trap !=TRAP_OK){
      fprintf(stderr,"trap activated: %s\n",trap_as_cstr(trap));
      vm_dump(stderr,&machine);
      exit(-1);
    }
  }
}
