
#include "vm.h"
#include "file.h"
vm machine = {0};
label_table lt ={0};
char* shift(int* argc,char *** argv){
  assert(*argc > 0);
  char* result = **argv;
  *argv +=1;
  *argc -=1;
  return result;
}

void usage(FILE* stream,const char* program){
  fprintf(stream,"Usage:./%s <input> <output>",program);
}

int main(int argc,char* argv[]) {
  if (argc < 3){
    fprintf(stderr,"Usage:./bme <input.vm> <output.vbm>");
    fprintf(stderr,"ERROR:expected input and output");
    exit(-1);
  }
  const char* input_file_path = argv[1];
  const char* output_file_path = argv[2];

  string_view src = slurp_file(input_file_path);
  translate_source(src,&machine,&lt);
  save_program_to_file(machine.program,machine.program_size,output_file_path);

}