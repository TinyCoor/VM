
#include "vm.h"
#include "file.h"
#include "assmbler.h"
context ctx= {0};

char *shift(int *argc, char ***argv) {
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

void usage(FILE *stream, const char *program) {
    fprintf(stream, "Usage:./%s <input> <output>", program);
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage:./bme <input.vm> <output.vbm>");
        fprintf(stderr, "ERROR:expected input and output");
        exit(-1);
    }
    const char *input_file_path = argv[1];
    const char *output_file_path = argv[2];


    assmble_source(cstr_as_string_view(input_file_path), &ctx, 0);
    save_program_to_file(&ctx, output_file_path);
    printf("%lld bytes used\n", ctx.arena_size);
}
