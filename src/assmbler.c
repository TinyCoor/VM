//
// Created by xm on 2021/8/27.
//
#include "vm.h"
#include "utils.h"

#define VM_COMMENT_SYMBOL ';'

void translate_source(string_view src,
                      vm* machine,
                      label_table* lt){
    machine->program_size = 0;
   // int line_number =0;
    while (src.count> 0) {
        assert(machine->program_size < PROGRAM_CAPACITY);
        string_view line = sv_trim(sv_chop_by_delim(&src, '\n'));
       // line_number += 1;
        if (line.count > 0 && *line.data != VM_COMMENT_SYMBOL) {
            string_view token = sv_chop_by_delim(&line, ' ');

            if (token.count > 0 && token.data[token.count - 1] == ':') {
                string_view label = {
                        token.count - 1,
                        token.data
                };
                label_table_push_label(lt, label, machine->program_size);

                token = sv_trim(sv_chop_by_delim(&line, ' '));
            }
            if (token.count > 0) {
                string_view op = sv_trim(sv_chop_by_delim(&line, VM_COMMENT_SYMBOL));

                inst_t inst_type = INST_NOP;
                if (names_to_type(token, &inst_type)) {
                    machine->program[machine->program_size].type = inst_type;
                    if (inst_has_op(inst_type)) {
                        if (!number_liter_as_word(op, &machine->program[machine->program_size].operand)) {
                            label_table_push_unresolved_label(lt, op, machine->program_size);
                        }
                    }
                    machine->program_size++;
                } else {
                    fprintf(stderr, "ERROR:Unkown instruction %.*s ", (int)token.count, token.data);
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
