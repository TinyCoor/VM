
#include "vm.h"
vm machine = {0};

int main(int argc,char* argv[]) {
  if (argc < 3){
    fprintf(stderr,"Usage:./bm <input.vm> <output.vbm>");
    fprintf(stderr,"ERROR:expected input and output");
    exit(-1);
  }
  const char* input_file_path = argv[1];
  const char* output_file_path = argv[2];

  string_view  src = slurp_file(input_file_path);
  machine.program_size = translate_src(src,
                                      machine.program,
                                      PROGRAM_CAPACITY);
  save_program_to_file(machine.program,machine.program_size,output_file_path);

}
