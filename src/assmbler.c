//
// Created by xm on 2021/8/27.
//
#include "vm.h"
#include "utils.h"

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
              .operand= number_liter_as_word(op)
          };
        } else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_DUP)))) {
          machine->program[machine->program_size++] = (inst) {
              .type = INST_DUP,
              .operand.as_u64 =sv_to_int(op)
          };
        } else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_ADDI)))) {
          machine->program[machine->program_size++] = (inst) {
              .type = INST_ADDI
          };
        } else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_JMP_IF)))) {
          if (op.count > 0 && isdigit(*(op.data))) {
            machine->program[machine->program_size++] = (inst) {
                INST_JMP_IF,
                .operand.as_u64 = sv_to_int(op)
            };
          } else {
            label_table_push_unresolved_label(lt, op, machine->program_size);
            machine->program[machine->program_size++] = (inst) {
                INST_JMP_IF,
            };
          }
        }else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_JMP)))) {
          if (op.count > 0 && isdigit(*(op.data))) {
            machine->program[machine->program_size++] = (inst) {
                INST_JMP,
                .operand.as_u64 = sv_to_int(op)
            };
          } else {
            label_table_push_unresolved_label(lt, op, machine->program_size);
            machine->program[machine->program_size++] = (inst) {
                INST_JMP,
            };
          }
        }else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_NOP)))) {
          machine->program[machine->program_size++] = (inst) {
              INST_NOP,
          };
        }else if (sv_eq(inst_name, cstr_as_string_view(inst_names(INST_DROP)))) {
          machine->program[machine->program_size++] = (inst) {
              INST_DROP,
          };
        } else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_ADDF)))){
          machine->program[machine->program_size++] = (inst){
              INST_ADDF,
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_SUBF)))){
          machine->program[machine->program_size++] = (inst){
              INST_SUBF,
          };
        } else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_HALT)))){
          machine->program[machine->program_size++] = (inst){
              INST_HALT
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_DIVF)))){
          machine->program[machine->program_size++] = (inst){
              INST_DIVF
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_MULF)))){
          machine->program[machine->program_size++] = (inst){
              INST_MULF
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_SWAP)))){
          machine->program[machine->program_size++] = (inst){
              INST_SWAP,
              .operand ={.as_u64=sv_to_int(op)}
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_DIVI)))){
          machine->program[machine->program_size++] = (inst){
              INST_DIVI,
              .operand =number_liter_as_word(op)
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_MULI)))){
          machine->program[machine->program_size++] = (inst){
              INST_MULI
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_NOT)))){
          machine->program[machine->program_size++] = (inst){
              INST_NOT
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_EQ)))){
          machine->program[machine->program_size++] = (inst){
              INST_EQ
          };
        } else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_GEF)))){
          machine->program[machine->program_size++] = (inst){
              INST_GEF
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_PRINT_DEBUG)))){
          machine->program[machine->program_size++] = (inst){
              INST_PRINT_DEBUG
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_RET)))){
          machine->program[machine->program_size++] = (inst){
              INST_RET
          };
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_CALL)))){
          if (op.count > 0 && isdigit(*(op.data))) {
            machine->program[machine->program_size++] = (inst) {
                INST_CALL,
                .operand.as_u64 = sv_to_int(op)
            };
          } else {
            label_table_push_unresolved_label(lt, op, machine->program_size);
            machine->program[machine->program_size++] = (inst) {
                INST_CALL,
            };
          }
        }else if (sv_eq(inst_name,cstr_as_string_view(inst_names(INST_FFI)))){
          machine->program[machine->program_size++] = (inst){
              INST_FFI,
              .operand.as_u64 = sv_to_int(op)
          };

        }
        else {
          fprintf(stderr, "ERROR:Unkown instruction %.*s ", inst_name.count, inst_name.data);
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