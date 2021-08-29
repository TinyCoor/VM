//
// Created by xm on 2021/8/15.
//
#include "file.h"

void save_program_to_file(inst *program,
                          size_t program_size,
                          const char *file_path) {
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n", file_path, strerror(errno));
        exit(-1);
    }

    fwrite(program, sizeof(program[0]), program_size, file);
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
    if (fseek(file, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR:Could not read file %s :%s\n", file_name, strerror(errno));
        exit(-1);
    }
    int pos = ftell(file);
    if (pos < 0) {
        fprintf(stderr, "ERROR:Could not read file %s :%s\n", file_name, strerror(errno));
        exit(-1);
    }
    assert(pos % sizeof(machine->program[0]) == 0);
    assert(pos <= PROGRAM_CAPACITY * sizeof(machine->program[0]));
    if (fseek(file, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR:Could not read file %s :%s\n", file_name, strerror(errno));
        exit(-1);
    }
    machine->program_size = fread(machine->program, sizeof(machine->program[0]), pos / sizeof(machine->program[0]),
                                  file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not read file %s :%s\n", file_name, strerror(errno));
        exit(-1);
    }
    fclose(file);
}

void load_program_from_memory(vm *machine, inst *program, size_t program_size) {
    assert(program_size < PROGRAM_CAPACITY);
    memcpy(machine->program, program, sizeof(inst) * program_size);
    machine->program_size = program_size;
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

    char *buffer = malloc(pos);

    size_t n = fread(buffer, 1, pos, file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    fclose(file);
    free(file_path_cstr);
    return (string_view) {n, buffer};
}

string_view label_table_slurp_file(label_table* lt,string_view file_path){
    char *file_path_cstr = label_table_alloc_memory(lt,file_path.count + 1);
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

    char *buffer = label_table_alloc_memory(lt,pos);
    size_t n = fread(buffer, 1, pos, file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR:Could not open file %s :%s\n",
                file_path_cstr, strerror(errno));
        exit(-1);
    }
    fclose(file);
    return (string_view) {n, buffer};
}
