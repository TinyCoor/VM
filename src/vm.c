//
// Created by xm on 2021/7/20.
//

#include "vm.h"


void push_inst(vm* machine,inst ins){
  assert(machine->program_size <PROGRAM_CAPACITY);
  machine->program[machine->program_size++] = ins;
}

err_t vm_execute_inst(vm* machine){
  if (machine->ip > machine->program_size){
    return ERR_ILLEGAL_INST_ACCESS;
  }
  inst vm_inst= machine->program[machine->ip];
  switch (vm_inst.type){
  case INST_NOP:{
    machine->ip+=1;
  }break;
  case INST_PUSH: {
    if(machine->stack_size >= VM_STACK_CAPACITY){
      return ERR_STACK_OVERFLOW;
    }
    machine->ip +=1;
    machine->stack[machine->stack_size++]=vm_inst.operand;
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
      machine->stack_size-=1;
      machine->ip = vm_inst.operand.as_u64;
    } else{
      machine->ip += 1;
    }
  }break;
  case INST_EQ:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2].as_u64 =machine->stack[machine->stack_size-1].as_u64== machine->stack[machine->stack_size-2].as_u64;
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_PRINT_DEBUG:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    printf("%ld\n",machine->stack[machine->stack_size-1].as_u64);
    machine->stack_size-=1;
    machine->ip +=1;
  }break;
  case INST_DUP:{
    if (machine->stack_size >VM_STACK_CAPACITY){
      return ERR_STACK_OVERFLOW;
    }
    if(machine->stack_size - vm_inst.operand.as_u64 <=0){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size] = machine->stack[machine->stack_size-1 -vm_inst.operand.as_u64];
    machine->stack_size+=1;
    machine->ip +=1;
  }break;

  case AMOUNT_OF_INSTS:
  default:
    return ERR_ILLEGAL_INST;
  }
  return ERR_OK;
}

void vm_dump_stack(FILE * stream,const vm* machine){
  printf("Stack:\n");
  if (machine->stack_size >0) {
    for (size_t i = 0; i < machine->stack_size; ++i) {
      fprintf(stream,"%lu %lf %ld %p\n",
              machine->stack[i].as_u64,
              machine->stack[i].as_f64,
              machine->stack[i].as_i64,
              machine->stack[i].as_ptr);
    }
  } else{
    fprintf(stream,"[empty]\n");
  }
}

void translate_source(string_view src,
                       vm* machine,
                       label_table* lt){
  while (src.count> 0){
    assert(machine->program_size < PROGRAM_CAPACITY);
    string_view line =sv_trim(sv_chop_by_delim(&src,'\n'));
    if (line.count > 0 && *line.data != '#'){
      string_view inst_name = sv_chop_by_delim(&line,' ');

      if (inst_name.count > 0 && inst_name.data[inst_name.count -1] ==':'){
        string_view label={
            inst_name.count-1,
            inst_name.data
        };
        label_table_push_label(lt,label,machine->program_size);

        inst_name = sv_trim( sv_chop_by_delim(&line,' '));
      }
      string_view op = sv_trim(sv_chop_by_delim(&line,'#'));

      if (inst_name.count >0) {
        if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_PUSH)))) {
          machine->program[machine->program_size++] = (inst) {
              INST_PUSH,
              sv_to_int(op)
          };
        } else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_DUP)))) {
          machine->program[machine->program_size++] = (inst) {
              INST_DUP,
              sv_to_int(op)
          };
        } else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_ADDI)))) {
          machine->program[machine->program_size++] = (inst) {
              INST_ADDI
          };
        } else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_JMP)))) {
          if (op.count > 0 && isdigit(*(op.data))) {
            machine->program[machine->program_size++] = (inst) {
                INST_JMP,
                sv_to_int(op)
            };
          } else {
            label_table_push_unresolved_label(lt, op, machine->program_size);
            machine->program[machine->program_size++] = (inst) {
                INST_JMP,
            };
          }
        } else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_NOP)))) {
          machine->program[machine->program_size++] = (inst) {
              INST_NOP,
          };
        } else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_ADDF)))){
          machine->program[machine->program_size++] = (inst){
            INST_ADDF
          };
        }
        else {
          fprintf(stderr, "ERROR:Unkonw instruction %.*s ", inst_name.count, inst_name.data);
          exit(-1);
        }
      }
      }
    }
  //Second pass
  for (int i = 0; i <lt->unresolved_size ; ++i) {
   inst_addr address = label_table_find_addr(lt,lt->unresolved_labels[i].name);
   machine->program[lt->unresolved_labels[i].addr].operand.as_u64 = address;
  }
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

