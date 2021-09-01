//
// Created by xm on 2021/8/7.
//
#include "vm.h"
#include "file.h"
#include "utils.h"
#include <inttypes.h>

vm machine={0};

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

int main(int argc,char** argv){
  char* program = shift(&argc,&argv);
  const char* input_file_path =NULL;
  int limit =-1;
  const char* flag =NULL;
  int debug = 0;
  while (argc >0){
    flag = shift(&argc,&argv);
    if (strcmp(flag,"-i") ==0){
      if (argc ==0 ){
        usage(stderr,program);
        fprintf(stderr,"no arg is provided for flag %s\n",flag);
        exit(-1);
      }
      input_file_path=shift(&argc,&argv);
    } else if (strcmp(flag,"-l") ==0){
      if (argc ==0 ){
        usage(stderr,program);
        fprintf(stderr,"no arg is provided for flag %s\n",flag);
        exit(-1);
      }
      limit = atoi(shift(&argc,&argv));
    } else if (strcmp(flag,"-d")==0){
      debug =1;
    } else if(strcmp(flag,"-h") == 0){
      usage(stdout,program);
      exit(0);
    }
    else{
      fprintf(stderr,"unkonwn flag %s\n",flag);
      exit(-1);
    }
  }
  if (input_file_path ==NULL){
    fprintf(stderr,"no input %s\n",flag);
    exit(-1);
  }
  load_program_from_file(&machine,input_file_path);

  //TODO some sort of mechanism to load function from dll
  push_native_fun(&machine,vm_malloc);      //0
  push_native_fun(&machine,vm_free);        //1
  push_native_fun(&machine,vm_print_f64);   //2
  push_native_fun(&machine,vm_print_i64);   //3
  push_native_fun(&machine,vm_print_u64);   //4
  push_native_fun(&machine,vm_print_ptr);   //5
  push_native_fun(&machine,vm_dump_memory); //6

  if (!debug){
    err_t  err = vm_execute_program(&machine,limit);
    //vm_dump_stack(stdout,&machine);
    if (err !=ERR_OK){
      fprintf(stderr,"ERROR:%s\n",err_as_cstr(err));
      exit(-1);
    }
  } else{
    while (limit!= 0 && !machine.halt) {
      vm_dump_stack(stdout,&machine);

      getchar();
      err_t err = vm_execute_inst(&machine);
        printf("%s %" PRIu64 "\n",
               inst_names( machine.program[machine.ip].type),
               machine.program[machine.ip].operand.as_u64
        );
      if (err !=ERR_OK){
        fprintf(stderr,"ERROR:%s\n",err_as_cstr(err));
        exit(-1);
      }
      if (limit >0){
        --limit;
      }
    }
  }
  return 0;
}
