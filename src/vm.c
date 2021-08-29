//
// Created by xm on 2021/7/20.
//

#include "vm.h"
#include <inttypes.h>
err_t vm_execute_inst(vm* machine){
  if (machine->ip > machine->program_size){
    return ERR_ILLEGAL_INST_ACCESS;
  }
  inst vm_inst= machine->program[machine->ip];
 // printf("current ip:%d current stack_size: %d \n",machine->ip,machine->stack_size);

  switch (vm_inst.type){
  case INST_NOP:{
    machine->ip+=1;
  }break;
  case INST_PUSH: {
    if(machine->stack_size >= VM_STACK_CAPACITY){
      return ERR_STACK_OVERFLOW;
    }
    machine->ip +=1;
    machine->stack[machine->stack_size++] = vm_inst.operand;
  }break;
  case INST_ADDI: {
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_u64 +=machine->stack[machine->stack_size-1].as_u64;
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_SUBI:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_u64 -=machine->stack[machine->stack_size-1].as_u64;
    machine->stack_size -=1;
    machine->ip += 1;
  }break;
  case INST_MULI: {
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_u64 *=machine->stack[machine->stack_size-1].as_u64;
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_DIVI:  {
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    if(machine->stack[machine->stack_size-1].as_u64 ==0){
      return ERR_DIV_BY_ZERO;
    }
    machine->stack[machine->stack_size-2].as_u64 /=machine->stack[machine->stack_size-1].as_u64;
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_ADDF:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_f64 +=machine->stack[machine->stack_size-1].as_f64;
    machine->stack_size -= 1;
    machine->ip += 1;
  }break;
  case INST_SUBF:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_f64 -=machine->stack[machine->stack_size-1].as_f64;
    machine->stack_size -= 1;
    machine->ip += 1;
  }break;
  case INST_MULF:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_f64 *=machine->stack[machine->stack_size-1].as_f64;
    machine->stack_size -= 1;
    machine->ip += 1;
  }break;
  case INST_DIVF:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_f64 /=machine->stack[machine->stack_size-1].as_f64;
    machine->stack_size -= 1;
    machine->ip += 1;
  }break;
  case INST_JMP:{
    machine->ip = vm_inst.operand.as_u64;
  }break;
  case INST_HALT:{
    machine->halt =1;
  }break;
  case INST_JMP_IF:{
    if (machine->stack_size <1){
      return ERR_STACK_UNDERFLOW;
    }
    if (machine->stack[machine->stack_size-1].as_u64){
      machine->ip = vm_inst.operand.as_u64;
    } else{
      machine->ip += 1;
    }
    machine->stack_size-=1;
  }break;
  case INST_GEF:{
    if (machine->stack_size < 2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_u64 =machine->stack[machine->stack_size-1].as_f64 >= machine->stack[machine->stack_size-2].as_f64;
    machine->stack_size -= 1;
    machine->ip+=1;
  }break;
  case INST_EQ:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_u64 =machine->stack[machine->stack_size-1].as_u64== machine->stack[machine->stack_size-2].as_u64;
    machine->stack_size -=1;
    machine->ip+=1;
  }break;

  case INST_DUP:{
    if (machine->stack_size >VM_STACK_CAPACITY){
      return ERR_STACK_OVERFLOW;
    }
    if(machine->stack_size - vm_inst.operand.as_u64 <=0){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size] = machine->stack[machine->stack_size-1 -vm_inst.operand.as_u64];
    machine->stack_size += 1;
    machine->ip += 1;
  }break;
  case INST_DROP:{
    if (machine->stack_size <1){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack_size -= 1;
    machine->ip += 1;
  }break;
  case INST_FFI:{
    if (vm_inst.operand.as_u64 > FFI_TABLE_CAPACITY){
      return ERR_ILLEGAL_OPERAND;
    }
    machine->ffi[vm_inst.operand.as_u64](machine);
    machine->ip  += 1;
  }break;
  case INST_SWAP:{
    if (vm_inst.operand.as_u64 >= machine->stack_size){
      return ERR_STACK_UNDERFLOW;
    }
    const uint64_t a = machine->stack_size -1;
    const uint64_t b = machine->stack_size -1 -vm_inst.operand.as_u64;
    Word t =machine->stack[a];
    machine->stack[a] = machine->stack[b];
    machine->stack[b] =t;
    machine->ip +=1;
  }break;
  case INST_ANDB:{
      if (machine->stack_size <2){
          return ERR_STACK_UNDERFLOW;
      }

      machine->stack[machine->stack_size -2 ].as_u64 = machine->stack[machine->stack_size -2].as_u64
                                                       & machine->stack[machine->stack_size -1].as_u64;
      machine->stack_size -= 1;
      machine->ip += 1;
  }break;
  case INST_XORB:{
      if (machine->stack_size <2){
          return ERR_STACK_UNDERFLOW;
      }

      machine->stack[machine->stack_size -2 ].as_u64 = machine->stack[machine->stack_size -2].as_u64
              ^ machine->stack[machine->stack_size -1].as_u64;
      machine->stack_size -= 1;
      machine->ip += 1;
  }break;
  case INST_SHR:{
      if (machine->stack_size <2){
          return ERR_STACK_UNDERFLOW;
      }

      machine->stack[machine->stack_size -2 ].as_u64 = machine->stack[machine->stack_size -2].as_u64
              >> machine->stack[machine->stack_size -1].as_u64;
      machine->stack_size -= 1;
      machine->ip += 1;
  }break;
  case INST_SHL:{
      if (machine->stack_size <2){
          return ERR_STACK_UNDERFLOW;
      }

      machine->stack[machine->stack_size -2 ].as_u64 = machine->stack[machine->stack_size -2].as_u64 <<
                                                                                       machine->stack[machine->stack_size -1].as_u64;
      machine->stack_size -= 1;
      machine->ip += 1;
  }break;
  case INST_ORB:{
      if (machine->stack_size <2){
          return ERR_STACK_UNDERFLOW;
      }
      machine->stack[machine->stack_size -2 ].as_u64 = machine->stack[machine->stack_size -2].as_u64 |
                                                                                                     machine->stack[machine->stack_size -1].as_u64;
      machine->stack_size -= 1;
      machine->ip += 1;
  }break;
  case INST_RET:{
    if (machine->stack_size <1){
      return ERR_STACK_UNDERFLOW;
    }
    machine->ip = machine->stack[machine->stack_size -1].as_u64;
    machine->stack_size -= 1;
  }break;

  case INST_CALL:{
    if (machine->stack_size <1){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size++].as_u64 =  machine->ip + 1;
    machine->ip =vm_inst.operand.as_u64;
  }break;

  case INST_NOT:{
    if (machine->stack_size > VM_STACK_CAPACITY){
      return ERR_STACK_OVERFLOW;
    }
    machine->stack[machine->stack_size-1].as_u64 = ! (machine->stack[machine->stack_size-1].as_u64);

    machine->ip += 1;
  }break;
  case AMOUNT_OF_INSTS:
  default:
    return ERR_ILLEGAL_INST;
  }
  return ERR_OK;
}


err_t vm_execute_program(vm* machine,int limit){
  while (limit!= 0 && !machine->halt) {
    err_t err = vm_execute_inst(machine);
    if (err !=ERR_OK){
      return err;
    }
    if (limit >0){
      --limit;
    }
  }
  return ERR_OK;
}

