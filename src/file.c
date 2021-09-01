//
// Created by xm on 2021/8/15.
//
#include "file.h"

void save_program_to_file(context * ctx,
                          const char *file_path) {
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n", file_path, strerror(errno));
        exit(-1);
    }
    file_meta_data data ={
            .magic =MAGIC,
            .version = VERSION,
            .program_size = ctx->program_size,
            .memory_size = ctx->memory_size,
            .memory_capacity = ctx->memory_capacity
    };
    fwrite(&data,sizeof(file_meta_data),1,file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not open file `%s`:%s\n", file_path, strerror(errno));
        exit(-1);
    }
    fwrite(ctx->program, sizeof(ctx->program[0]), ctx->program_size, file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not open file `%s`:%s\n", file_path, strerror(errno));
        exit(-1);
    }
    fwrite(ctx->memory, sizeof(ctx->memory[0]), ctx->memory_capacity, file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not open file `%s`:%s\n", file_path, strerror(errno));
        exit(-1);
    }
    fclose(file);
}

void load_program_from_file(vm *machine, const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n", file_name, strerror(errno));
        exit(-1);
    }
    file_meta_data data ={0};
    size_t n = fread(&data,sizeof(data),1,file);
    if (n < 1){
        fprintf(stderr,"read file error\n");
        exit(-1);
    }
    if (data.magic != MAGIC){
        fprintf(stderr,"not support file format\n");
        exit(-1);
    }
    if (data.version != VERSION){
        fprintf(stderr,"ERROR: not support version\n");
        exit(-1);
    }
    if (data.program_size > PROGRAM_CAPACITY){
        fprintf(stderr,"ERROR: program section too big contans\n");
        exit(-1);
    }

    if (data.memory_capacity > MAX_STATIC_MEM){
        fprintf(stderr,"ERROR: mem section too big contans\n");
        exit(-1);
    }

    if (data.memory_size > data.memory_capacity){
        fprintf(stderr,"ERROR: mem_size > capacity\n");
        exit(-1);
    }
    machine->program_size = fread(machine->program,sizeof(machine->program[0]),data.program_size,file);
    if (machine->program_size != data.program_size){
        fprintf(stderr,"read program error\n");
        exit(-1);
    }
    machine->memory_size = fread(machine->memory,sizeof(machine->memory[0]),data.memory_size,file);
    if ( machine->memory_size != data.memory_size){
        fprintf(stderr,"read mem error\n");
        exit(-1);
    }

    fclose(file);
}



string_view slurp_file(string_view file_path) {
    char *file_path_cstr = malloc(file_path.count + 1);
    if (file_path_cstr == NULL) {
        fprintf(stderr, "ERROR:Could not alloc for file %.*s :%s\n",SV_FORMAT(file_path), strerror(errno));
        exit(-1);
    }
    memcpy(file_path_cstr, file_path.data, file_path.count);
    file_path_cstr[file_path.count] = '\0';
    FILE *file = fopen(file_path_cstr, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    if (fseek(file, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n", file_path_cstr, strerror(errno));
        exit(-1);
    }
    long pos = ftell(file);
    if (pos < 0) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    if (fseek(file, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }

    char *buffer = malloc((size_t)pos);

    size_t n = fread(buffer, 1, (size_t)pos, file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    fclose(file);
    free(file_path_cstr);
    return (string_view) {n, buffer};
}

string_view ctx_slurp_file(context * lt,string_view file_path){
    char *file_path_cstr = ctx_alloc_memory(lt,file_path.count + 1);
    if (file_path_cstr == NULL) {
        fprintf(stderr, "ERROR:Could not alloc for file %.*s :%s\n",SV_FORMAT(file_path), strerror(errno));
        exit(-1);
    }
    memcpy(file_path_cstr, file_path.data, file_path.count);
    file_path_cstr[file_path.count] = '\0';
    FILE *file = fopen(file_path_cstr, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    if (fseek(file, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n", file_path_cstr, strerror(errno));
        exit(-1);
    }
    int pos = ftell(file);
    if (pos < 0) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    if (fseek(file, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }

    char *buffer = ctx_alloc_memory(lt,(size_t)pos);
    size_t n = fread(buffer, 1, (size_t)pos, file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    fclose(file);
    return (string_view) {n, buffer};
}
