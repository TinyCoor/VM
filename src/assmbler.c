//
// Created by xm on 2021/8/27.
//

#include "vm.h"
#include "utils.h"
#include "file.h"

const char COMMET_TYPE_SYMBOL =';';
const char PRE_PROCESSOR_SYMBOL = '%';
const char* BIND_SYMBOL ="bind";
const size_t MAX_INCLUE_LEVEL =10;

void assmble_source(string_view file_path,
                    context *ctx,
                    size_t level) {
    string_view original_src = ctx_slurp_file(ctx,file_path);
    string_view src = original_src;

    ctx->program_size = 0;
    ctx->memory_size = 0;
    ctx->memory_capacity = 0;
    int line_number = 0;

    while (src.count > 0) {
        string_view line = sv_trim(sv_chop_by_delim(&src, '\n'));
        line_number += 1;
        if (line.count > 0 && *line.data != COMMET_TYPE_SYMBOL) {
            string_view token = sv_chop_by_delim(&line, ' ');
            //Pre-process
            if (token.count > 0 && *token.data == PRE_PROCESSOR_SYMBOL) {
                token.count -= 1;
                token.data += 1;
                if (sv_eq(token, cstr_as_string_view(BIND_SYMBOL))) {
                    line = sv_trim(line);
                    string_view label = sv_chop_by_delim(&line, ' ');
                    if (label.count > 0) {
                        line = sv_trim(line);
                        string_view value =line;
                        Word word ={0};
                        if(!translate_literal(ctx,value,&word)){
                            fprintf(stderr, "%.*s:%d: %.*s is not a number \n ",
                                    (int)file_path.count,file_path.data, line_number,
                                    (int)label.count,label.data);
                            exit(-1);
                        }
                        if (!ctx_bind_label(ctx,label,word)){
                            fprintf(stderr, "%.*s:%d: ERROR name %.*s is binded \n ",
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
                            assmble_source(line,ctx,level +1);
                        }
                    }else{
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
                //Label binding
                if (token.count > 0 && token.data[token.count - 1] == ':') {
                    string_view label = {
                            .count = token.count - 1,
                            .data = token.data
                    };
                    if (!ctx_bind_label(ctx,label,(Word){ .as_u64 = ctx->program_size})){
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
                        assert(ctx->program_size < PROGRAM_CAPACITY);
                        ctx->program[ctx->program_size].type = inst_type;
                        if (inst_has_op(inst_type)) {
                            if (op.count == 0) {
                                fprintf(stderr,
                                        "%.*s:%d: ERROR:instruction Error %.*s  requires an operand",
                                SV_FORMAT(file_path), line_number,
                                 SV_FORMAT( token));
                                exit(-1);
                            }
                            if (!translate_literal(ctx,op, &ctx->program[ctx->program_size].operand)) {
                                ctx_push_deferred_label(ctx, op, ctx->program_size);
                            }
                        }
                        ctx->program_size++;
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
    for (int i = 0; i < ctx->deferred_size; ++i) {
        string_view label  =ctx->deferred_labels[i].name;
        if (!ctx_resolve_label(ctx,
                                      ctx->deferred_labels[i].name,
                                      &ctx->program[ctx->deferred_labels[i].addr].operand)){
           //TODO need to report location in the source code
            fprintf(stderr, "%.*s : ERROR:Unkown binding %.*s ",
            SV_FORMAT(file_path), (int) label.count, label.data);
            exit(-1);
        }
    }
}
