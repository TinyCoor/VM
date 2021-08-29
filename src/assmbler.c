//
// Created by xm on 2021/8/27.
//
#include "vm.h"
#include "utils.h"
#include "file.h"

#define VM_COMMENT_SYMBOL ';'
const char COMMET_TYPE_SYMBOL =';';
const char PRE_PROCESSOR_SYMBOL = '%';

void assmble_source(const char *file_path,
                    vm *machine,
                    label_table *lt) {
    string_view src = slurp_file(file_path);
    machine->program_size = 0;
    int line_number = 0;
    while (src.count > 0) {
        assert(machine->program_size < PROGRAM_CAPACITY);
        string_view line = sv_trim(sv_chop_by_delim(&src, '\n'));
        line_number += 1;
        if (line.count > 0 && *line.data != COMMET_TYPE_SYMBOL) {
            string_view token = sv_chop_by_delim(&line, ' ');
            //Pre-process
            if (token.count > 0 && *token.data == PRE_PROCESSOR_SYMBOL) {
                token.count -= 1;
                token.data += 1;
                if (sv_eq(token, cstr_as_string_view("label"))) {
                    line = sv_trim(line);
                    string_view label = sv_chop_by_delim(&line, ' ');
                    if (label.count > 0) {
                        line = sv_trim(line);
                        string_view label_name = sv_chop_by_delim(&line, ' ');
                        Word word ={0};
                        if(!number_liter_as_word(label_name,&word)){
                            fprintf(stderr, "%s:%d:  %.*s is not a number \n ",
                                    file_path, line_number,(int)label_name.count,label_name.data);
                            exit(-1);
                        }
                        label_table_push_label(lt,label,word.as_u64);
                    } else {
                        fprintf(stderr, "%s:%d: ERROR label is not provided \n ",
                                file_path, line_number);
                        exit(-1);
                    }
                } else {
                    fprintf(stderr, "%s:%d: ERROR:Unkown pre-process directive %.*s \n",
                            file_path, line_number,
                            (int) token.count,
                            token.data);
                    exit(-1);
                }
            } else {
                //Label
                if (token.count > 0 && token.data[token.count - 1] == ':') {
                    string_view label = {
                            .count = token.count - 1,
                            .data = token.data
                    };
                    label_table_push_label(lt, label, machine->program_size);

                    token = sv_trim(sv_chop_by_delim(&line, ' '));
                }

                //Instruction
                if (token.count > 0) {
                    string_view op = sv_trim(sv_chop_by_delim(&line, COMMET_TYPE_SYMBOL));

                    inst_t inst_type = INST_NOP;
                    if (names_to_type(token, &inst_type)) {
                        machine->program[machine->program_size].type = inst_type;
                        if (inst_has_op(inst_type)) {
                            if (op.count == 0) {
                                fprintf(stderr,
                                        "%s:%d: ERROR:instruction Error %.*s  requires an operand",
                                        file_path, line_number,
                                        (int) token.count, token.data);
                                exit(-1);
                            }
                            if (!number_liter_as_word(op, &machine->program[machine->program_size].operand)) {
                                label_table_push_unresolved_label(lt, op, machine->program_size);
                            }
                        }
                        machine->program_size++;
                    } else {
                        fprintf(stderr, "%s:%d: ERROR:Unkown instruction %.*s ",
                                file_path, line_number, (int) token.count, token.data);
                        exit(-1);
                    }
                }
            }
        }
    }
    //Second pass
    for (int i = 0; i < lt->unresolved_size; ++i) {
        inst_addr address = label_table_find_addr(lt, lt->unresolved_labels[i].name);
        machine->program[lt->unresolved_labels[i].addr].operand.as_u64 = address;
    }
}
