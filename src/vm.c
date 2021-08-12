//
// Created by xm on 2021/7/20.
//

#include "vm.h"
const char* err_as_cstr(err_t trap){
  switch (trap) {
  case ERR_OK:{return "ERR_OK";}
  case ERR_STACK_OVERFLOW:{return "ERR_STACK_OVERFLOW";}
  case ERR_STACK_UNDERFLOW:{return "ERR_STACK_UNDERFLOW";}
  case ERR_ILLEGAL_INST:{return "ERR_ILLEGAL_INST";}
  case ERR_DIV_BY_ZERO:{return "ERR_DIV_BY_ZERO";}
  case ERR_ILLEGAL_OPERAND:{return "ERR_ILLEGAL_OPERAND";}
  case ERR_ILLEGAL_INST_ACCESS:{return "ERR_ILLEGAL_INST_ACCESS";}
  default:{assert(0 &&"trap_as_cstr: Unreachable");}
  }
}
const char* inst_type_as_cstr(inst_t inst_type){
  switch (inst_type) {
  case INST_NOP:{return "INST_NOP";}
  case INST_PUSH:{return "INST_PUSH";}
  case INST_PLUS:{return "INST_PLUS";}
  case INST_MULT:{return "INST_MULT";}
  case INST_DIV:{return "INST_DIV";}
  case INST_MINUS:{return "INST_MINUS";}
  case INST_JMP:{return "INST_JMP";}
  case INST_HALT:{return "INST_HALT";}
  case INST_JMP_IF:{return "INST_JMP_IF";}
  case INST_EQ:{return "INST_EQ";}
  case INST_PRINT_DEBUG:{return "INST_PRINT_DEBUG";}
  case INST_DUP:{return "INST_DUP";}
  default:assert(0&& "inst_type_as_cstr:Unreachable");
  }
}
void push_inst(vm* machine,inst ins){
  assert(machine->program_size <PROGRAM_CAPACITY);
  machine->program[machine->program_size++] = ins;
}
err_t vm_execute_inst(vm* machine){
  if (machine->ip <0 || machine->ip > machine->program_size){
    return ERR_ILLEGAL_INST_ACCESS;
  }
  inst vm_inst= machine->program[machine->ip];
  switch (vm_inst.type){
  case INST_NOP:{
    machine->ip+=1;
  }break;
  case INST_PUSH: {
    if(machine->stack_size >= VM_STACK_CAPACITY){
      return ERR_STACK_OVERFLOW;
    }
    machine->ip +=1;
    machine->stack[machine->stack_size++]=vm_inst.operand;
  }break;
  case INST_PLUS: {
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2] +=machine->stack[machine->stack_size-1];
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_MINUS:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2] -=machine->stack[machine->stack_size-1];
    machine->stack_size -=1;
  }break;
  case INST_MULT: {
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2] *=machine->stack[machine->stack_size-1];
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_DIV:  {
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    if(machine->stack[machine->stack_size-1] ==0){
      return ERR_DIV_BY_ZERO;
    }
    machine->stack[machine->stack_size-2] /=machine->stack[machine->stack_size-1];
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_JMP:{
    machine->ip = vm_inst.operand;
  }break;
  case INST_HALT:{
    machine->halt =1;
  }break;
  case INST_JMP_IF:{
    if (machine->stack_size <1){
      return ERR_STACK_UNDERFLOW;
    }
    if (machine->stack[machine->stack_size-1]){
      machine->stack_size-=1;
      machine->ip = vm_inst.operand;
    } else{
      machine->ip += 1;
    }
  }break;
  case INST_EQ:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    machine->stack[machine->stack_size-2] =machine->stack[machine->stack_size-1]== machine->stack[machine->stack_size-2];
    machine->stack_size -=1;
    machine->ip+=1;
  }break;
  case INST_PRINT_DEBUG:{
    if (machine->stack_size <2){
      return ERR_STACK_UNDERFLOW;
    }
    printf("%ld\n",machine->stack[machine->stack_size-1]);
    machine->stack_size-=1;
    machine->ip +=1;
  }break;
  case INST_DUP:{
    if (machine->stack_size >VM_STACK_CAPACITY){
      return ERR_STACK_OVERFLOW;
    }
    if(machine->stack_size -vm_inst.operand <=0){
      return ERR_STACK_UNDERFLOW;
    }
    if (vm_inst.operand <0){
      return ERR_ILLEGAL_OPERAND;
    }
    machine->stack[machine->stack_size] = machine->stack[machine->stack_size-1 -vm_inst.operand];
    machine->stack_size+=1;
    machine->ip +=1;
  }break;
  default:
    return ERR_ILLEGAL_INST;
  }
  return ERR_OK;
}
//err_t get_stack_frame(vm* machine){
//
//}

void vm_dump_stack(FILE * stream,const vm* machine){
  printf("Stack:\n");
  if (machine->stack_size >0) {
    for (size_t i = 0; i < machine->stack_size; ++i) {
      fprintf(stream,"%lld\n", machine->stack[i]);
    }
  } else{
    fprintf(stream,"[empty]\n");
  }
}
void load_program_from_memory(vm* machine,inst* program,size_t program_size){
  assert(program_size <PROGRAM_CAPACITY);
  memcpy(machine->program,program,sizeof(inst)*program_size);
  machine->program_size=program_size;
}

void save_program_to_file(inst* program,
                          size_t program_size,
                          const char* file_path) {
  FILE* file =fopen(file_path,"wb");
  if (file ==NULL){
    fprintf(stderr,"ERROR:Could not open file %s :%s\n",file_path,strerror(errno));
    exit(-1);
  }

  fwrite(program,sizeof(program[0]),program_size,file);
  if (ferror(file)){
    fprintf(stderr,"ERROR:Could not open file `%s`:%s\n",file_path,strerror(errno));

    exit(-1);
  }
  fclose(file);
}

void load_program_from_file(vm* machine, const char* file_name){
  FILE* file= fopen(file_name,"rb");
  if (file == NULL){
    fprintf(stderr,"ERROR:Could not open file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  if (fseek(file,0,SEEK_END)< 0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  int pos = ftell(file);
  if (pos <0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  assert(pos % sizeof(machine->program[0]) == 0);
  assert(pos <= PROGRAM_CAPACITY*sizeof(machine->program[0]));
  if (fseek(file,0,SEEK_SET) <0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  machine->program_size = fread(machine->program,sizeof(machine->program[0]),pos/sizeof(machine->program[0]),file);
  if (ferror(file)){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  fclose(file);
}

inst translate_line(vm* machine,label_table* lt,string_view line){
  line= sv_trim_left(line);
  string_view inst_name = sv_chop_by_delim(&line,' ');
  string_view op = sv_trim(sv_chop_by_delim(&line,'#'));

  if (inst_name.count > 0 && inst_name.data[inst_name.count -1] ==':'){
    string_view label={inst_name.count-1,inst_name.data};
    label_table_push(lt,label,machine->program_size);
  } else if (sv_eq(inst_name,cstr_as_string_view("push"))){
    line= sv_trim_left(line);
    int op = sv_to_int(sv_trim_right(line));
    return (inst){INST_PUSH,op};
  } else if(sv_eq(inst_name,cstr_as_string_view("dup"))){
    line = sv_trim_left(line);
    return (inst){INST_DUP,sv_to_int(op)};
  } else if (sv_eq(inst_name,cstr_as_string_view("plus"))){
    return (inst){INST_PLUS};
  } else if (sv_eq(inst_name,cstr_as_string_view("jmp"))){
    line = sv_trim_left(line);
    return (inst){INST_JMP,sv_to_int(op)};
  }else{
    fprintf(stderr,"ERROR:Unkonw instruction %.*s ",inst_name.count,inst_name.data);
  }

  return MAKE_INST_NOP;
}

void translate_src(string_view src,
                     vm* machine,
                     label_table* lt){
  while (src.count> 0){
    assert(machine->program_size < PROGRAM_CAPACITY);
    string_view line =sv_trim(  sv_chop_by_delim(&src,'\n'));
    if (line.count > 0 && *line.data!='#'){
      const inst ins = translate_line(machine,lt,line);
      machine->program[machine->program_size++] = ins;
    }
  }
  print_label_table(lt);
}

void translate_source(string_view src,
                       vm* machine,
                       label_table* lt){
  while (src.count> 0){
    assert(machine->program_size < PROGRAM_CAPACITY);
    string_view line =sv_trim(  sv_chop_by_delim(&src,'\n'));
    if (line.count > 0 && *line.data!='#'){
      string_view inst_name = sv_chop_by_delim(&line,' ');
      string_view op = sv_trim(sv_chop_by_delim(&line,'#'));
      if (inst_name.count > 0 && inst_name.data[inst_name.count -1] ==':'){
        string_view label={inst_name.count-1,inst_name.data};
        label_table_push(lt,label,machine->program_size);
      } else if (sv_eq(inst_name,cstr_as_string_view("push"))){
        line= sv_trim_left(line);
        machine->program[machine->program_size++] = (inst){
          INST_PUSH,
          sv_to_int(op)
        };
      } else if(sv_eq(inst_name,cstr_as_string_view("dup"))){
        line = sv_trim_left(line);
        machine->program[machine->program_size++] = (inst){
          INST_DUP,
          sv_to_int(op)
        };
      } else if (sv_eq(inst_name,cstr_as_string_view("plus"))){
        machine->program[machine->program_size++] = (inst){
          INST_PLUS
        };
      } else if (sv_eq(inst_name,cstr_as_string_view("jmp"))){
        label_table_push_unresolved_label(lt,op,machine->program_size);
        line = sv_trim_left(line);
        machine->program[machine->program_size++] = (inst){
          INST_JMP,
        };
      }else{
        fprintf(stderr,"ERROR:Unkonw instruction %.*s ",inst_name.count,inst_name.data);
        exit(-1);
      }
    }
  }

  for (int i = 0; i <lt->unresolved_size ; ++i) {
   word address = label_table_find(lt,lt->unresolved_labels[i].name);
   machine->program[lt->unresolved_labels[i].addr].operand = address;
  }
}

string_view slurp_file(const char* file_name){
  FILE* file= fopen(file_name,"rb");
  if (file == NULL){
    fprintf(stderr,"ERROR:Could not open file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  if (fseek(file,0,SEEK_END)< 0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  int pos = ftell(file);
  if (pos <0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }
  if (fseek(file,0,SEEK_SET) <0){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }

  char* buffer = malloc(pos);

  size_t n = fread(buffer,1,pos,file);
  if (ferror(file)){
    fprintf(stderr,"ERROR:Could not read file %s :%s\n",file_name,strerror(errno));
    exit(-1);
  }

  fclose(file);
  return (string_view){n,buffer};
}

err_t vm_execute_program(vm* machine,int limit){
  while (limit!= 0 && !machine->halt) {
    err_t err = vm_execute_inst(machine);
    if (err !=ERR_OK){
      return err;
    }
    if (limit >0){
      --limit;
    }
  }
  return ERR_OK;
}

