#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

#define VM_STACK_CAPACITY 1024
#define PROGRAM_CAPACITY 1024
#define ARRAY_SIZE(xs) (sizeof(xs)/sizeof((xs)[0]))
#define EXEC_LIMIT 1024
typedef int64_t word;

typedef enum {
  ERR_OK,
  ERR_STACK_OVERFLOW,
  ERR_STACK_UNDERFLOW,
  ERR_ILLEGAL_INST,
  ERR_ILLEGAL_OPERAND,
  ERR_DIV_BY_ZERO,
  ERR_ILLEGAL_INST_ACCESS

}err_t;

const char* err_as_cstr(err_t trap){
  switch (trap) {
    case ERR_OK:{return "ERR_OK";}
    case ERR_STACK_OVERFLOW:{return "ERR_STACK_OVERFLOW";}
    case ERR_STACK_UNDERFLOW:{return "ERR_STACK_UNDERFLOW";}
    case ERR_ILLEGAL_INST:{return "ERR_ILLEGAL_INST";}
    case ERR_DIV_BY_ZERO:{return "ERR_DIV_BY_ZERO";}
    case ERR_ILLEGAL_OPERAND:{return "ERR_ILLEGAL_OPERAND";}
    case ERR_ILLEGAL_INST_ACCESS:{return "ERR_ILLEGAL_INST_ACCESS";}
    default:{assert(0 &&"trap_as_cstr: Unreachable");}
  }
}


typedef enum{
  INST_PUSH,
  INST_PLUS,
  INST_MINUS,
  INST_MULT,
  INST_DIV,
  INST_JMP,
  INST_JMP_IF,
  INST_EQ,
  INST_DUP,
  INST_HALT,
  INST_PRINT_DEBUG,
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
    case INST_JMP_IF:{return "INST_JMP_IF";}
    case INST_EQ:{return "INST_EQ";}
    case INST_PRINT_DEBUG:{return "INST_PRINT_DEBUG";}
    case INST_DUP:{return "INST_DUP";}
    default:assert(0&& "inst_type_as_cstr:Unreachable");
  }
}

typedef struct {
  inst_t type;
  word operand;
} inst;

typedef struct {
  word stack[VM_STACK_CAPACITY];
  word stack_size;

  inst program[PROGRAM_CAPACITY];
  word program_size;

  word ip;
  int halt;
} vm;

void push_inst(vm* machine,inst ins){
  assert(machine->program_size <PROGRAM_CAPACITY);
  machine->program[machine->program_size++] = ins;
}

err_t vm_execute_inst(vm* machine){
  if (machine->ip <0 || machine->ip > machine->program_size){
    return ERR_ILLEGAL_INST_ACCESS;
  }
  inst vm_inst= machine->program[machine->ip];
  switch (vm_inst.type) {
    case INST_PUSH: {
      if(machine->stack_size >= VM_STACK_CAPACITY){
        return ERR_STACK_OVERFLOW;
      }
      machine->ip +=1;
      machine->stack[machine->stack_size++]=vm_inst.operand;
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
      machine->ip = vm_inst.operand;
    }break;
    case INST_HALT:{
      machine->halt =1;
    }break;
    case INST_JMP_IF:{
      if (machine->stack_size <1){
        return ERR_STACK_UNDERFLOW;
      }
      if (machine->stack[machine->stack_size-1]){
        machine->stack_size-=1;
        machine->ip = vm_inst.operand;
      } else{
        machine->ip += 1;
      }
    }break;
    case INST_EQ:{
      if (machine->stack_size <2){
        return ERR_STACK_UNDERFLOW;
      }
      machine->stack[machine->stack_size-2] =machine->stack[machine->stack_size-1]== machine->stack[machine->stack_size-2];
      machine->stack_size -=1;
      machine->ip+=1;
    }break;
    case INST_PRINT_DEBUG:{
      if (machine->stack_size <2){
        return ERR_STACK_UNDERFLOW;
      }
      printf("%ld\n",machine->stack[machine->stack_size-1]);
      machine->stack_size-=1;
      machine->ip +=1;
    }break;
    case INST_DUP:{
      if (machine->stack_size >VM_STACK_CAPACITY){
        return ERR_STACK_OVERFLOW;
      }
      if(machine->stack_size -vm_inst.operand <=0){
        return ERR_STACK_UNDERFLOW;
      }
      if (vm_inst.operand <0){
        return ERR_ILLEGAL_OPERAND;
      }
      machine->stack[machine->stack_size] = machine->stack[machine->stack_size-1 -vm_inst.operand];
      machine->stack_size+=1;
      machine->ip +=1;
    }break;
  default:
    return ERR_ILLEGAL_INST;
  }
  return ERR_OK;
}

err_t get_stack_frame(vm* machine){

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
#define MAKE_INST_DUP(addr) ((inst){.type = INST_DUP,.operand=(addr)})
#define MAKE_INST_HALT(addr) ((inst){.type = INST_HALT,.operand=(addr)})

vm machine = {0};
inst program[]={
    MAKE_INST_PUSH(0),//0
    MAKE_INST_PUSH(1),//1
    MAKE_INST_DUP(1),//2
    MAKE_INST_DUP(1),//3
    MAKE_INST_PLUS,//4
    MAKE_INST_JMP(2)//5
};

void load_program_from_memory(vm* machine,inst* program,size_t program_size){
  assert(program_size <PROGRAM_CAPACITY);
  memcpy(machine->program,program,sizeof(inst)*program_size);
  machine->program_size=program_size;
}

void save_program_to_file(inst* program,
                          size_t program_size,
                          const char* file_path) {
  FILE* file =fopen(file_path,"wb");
  if (file ==NULL){
    fprintf(stderr,"ERROR:Could not open file %s :%s\n",file_path,strerror(errno));
    exit(-1);
  }

  fwrite(program,sizeof(program[0]),program_size,file);
  if (ferror(file)){
    fprintf(stderr,"ERROR:Could not open file `%s`:%s\n",file_path,strerror(errno));

    exit(-1);
  }
  fclose(file);
}

void load_program_from_file(vm* machine, const char* file_name){
  FILE* file= fopen(file_name,"rb");
  if (file == NULL){
    fprintf(stderr,"ERROR:Could not open file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  if (fseek(file,0,SEEK_END)< 0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  int pos = ftell(file);
  if (pos <0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  assert(pos %sizeof(machine->program[0]) == 0 );
  assert(pos <= PROGRAM_CAPACITY*sizeof(machine->program[0]));
  if (fseek(file,0,SEEK_SET) <0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  machine->program_size = fread(machine->program,sizeof(machine->program[0]),pos/sizeof(machine->program[0]),file);
  if (ferror(file)){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  fclose(file);
}



int main(int argc,char* argv[]) {
//  load_program_from_memory(&machine,program,ARRAY_SIZE(program));
  save_program_to_file(program,ARRAY_SIZE(program),"./fib.vm");
  load_program_from_file(&machine,"./fib.vm");
  for (int i =0; i< 69 && !machine.halt;++i){
    err_t trap =vm_execute_inst(&machine);
    vm_dump_stack(stdout,&machine);
    if (trap !=ERR_OK){
      fprintf(stderr,"ERROR: %s\n",err_as_cstr(trap));
      exit(-1);
    }
  }
  vm_dump_stack(stdout,&machine);
  return 0;
}
