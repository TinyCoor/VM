//
// Created by xm on 2021/8/27.
//

#include "vm.h"
#include "utils.h"
#include "file.h"

const char COMMET_TYPE_SYMBOL =';';
const char PRE_PROCESSOR_SYMBOL = '%';
const size_t MAX_INCLUE_LEVEL =10;

void assmble_source(string_view file_path,
                    vm *machine,
                    label_table *lt,
                    size_t level) {
    string_view original_src = label_table_slurp_file(lt,file_path);
    string_view src = original_src;

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
                            fprintf(stderr, "%.*s:%d: %.*s is not a number \n ",
                                    (int)file_path.count,file_path.data, line_number,
                                    (int)label.count,label.data);
                            exit(-1);
                        }
                        if (!label_table_bind_label(lt,label,word)){
                            fprintf(stderr, "%.*s:%d: ERROR label  %.*s is binded \n ",
                                    (int)file_path.count,file_path.data, line_number,
                                    (int)label.count,label.data);
                            exit(-1);
                        }
                    } else {
                        fprintf(stderr, "%.*s:%d: ERROR label is not provided \n ",
                                (int)file_path.count,file_path.data, line_number);
                        exit(-1);
                    }
                } else if(sv_eq(token, cstr_as_string_view("include"))){
                    line = sv_trim(line);
                    if (line.count > 0) {
                        if (*line.data == '\"' && line.data[line.count -1] == '\"'){
                            line.data += 1;
                            line.count -= 2;
                            if (level +1 >MAX_INCLUE_LEVEL){
                                fprintf(stderr, "%.*s:%d: exceeded max include level\n ",
                                SV_FORMAT(file_path),line_number);
                                exit(-1);
                            }
                            //遞歸
                            assmble_source(line,machine,lt,level +1);
                        }
                    } else{
                        fprintf(stderr, "%.*s:%d: ERROR include file path has surround with " "\n ",
                        SV_FORMAT(file_path),line_number);
                        exit(-1);
                    }

                }else {
                    fprintf(stderr, "%.*s:%d: ERROR:Unkown pre-process directive %.*s \n",
                    SV_FORMAT(file_path),line_number,SV_FORMAT(token));
                    exit(-1);
                }
            } else {
                //Label
                if (token.count > 0 && token.data[token.count - 1] == ':') {
                    string_view label = {
                            .count = token.count - 1,
                            .data = token.data
                    };
                    if (!label_table_bind_label(lt,label,(Word){ .as_u64 = machine->program_size})){
                        fprintf(stderr, "%.*s:%d: ERROR label  %.*s is binded \n ",
                        SV_FORMAT(file_path), line_number,
                        SV_FORMAT(label));
                        exit(-1);
                    }

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
                                        "%.*s:%d: ERROR:instruction Error %.*s  requires an operand",
                                SV_FORMAT(file_path), line_number,
                                 SV_FORMAT( token));
                                exit(-1);
                            }
                            if (!number_liter_as_word(op, &machine->program[machine->program_size].operand)) {
                                label_table_push_deferred_label(lt, op, machine->program_size);
                            }
                        }
                        machine->program_size++;
                    } else {
                        fprintf(stderr, "%.*s:%d: ERROR:Unkown instruction %.*s ",
                        SV_FORMAT(file_path), line_number, SV_FORMAT(token));
                        exit(-1);
                    }
                }
            }
        }
    }

    //Second pass
    for (int i = 0; i < lt->deferred_size; ++i) {
        string_view label  =lt->deferred_labels[i].name;
        if (!label_table_resolve_label(lt,
                                      lt->deferred_labels[i].name,
                                      &machine->program[lt->deferred_labels[i].addr].operand)){
           //TODO need to report location in the source code
            fprintf(stderr, "%.*s : ERROR:Unkown label %.*s ",
            SV_FORMAT(file_path), (int) label.count, label.data);
            exit(-1);
        }
    }
}
