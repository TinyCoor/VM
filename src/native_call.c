//
// Created by xm on 2021/8/28.
//

#include "native_call.h"
#include "vm.h"
#include <inttypes.h>

err_t vm_malloc(vm* machine){
  if (machine->stack_size <1){
    return ERR_STACK_UNDERFLOW;
  }
  machine->stack[machine->stack_size - 1].as_ptr = malloc(machine->stack[machine->stack_size -1].as_u64);

  return ERR_OK;
}

err_t vm_free(struct vm* machine){
  if (machine->stack_size <1){
    return ERR_STACK_UNDERFLOW;
  }
  free(machine->stack[machine->stack_size-1].as_ptr);
  machine->stack_size -= 1;
  return ERR_OK;
}

void push_native_fun(vm* machine,native_func func){
  assert(machine->ffi_size < FFI_TABLE_CAPACITY);
  machine->ffi[machine->ffi_size++] = func;
}

err_t vm_print_f64(struct vm* machine){
  if (machine->stack_size <1){
    return ERR_STACK_UNDERFLOW;
  }
  printf("%lf\n",machine->stack[machine->stack_size - 1].as_f64);
  machine->stack_size -= 1;
  return ERR_OK;
}

err_t vm_print_u64(struct vm* machine){
  if (machine->stack_size <1){
    return ERR_STACK_UNDERFLOW;
  }
  printf("%"PRIu64 "\n",machine->stack[machine->stack_size-1].as_u64);
  machine->stack_size -= 1;
  return ERR_OK;
}

err_t vm_print_ptr(struct vm* machine){
  if (machine->stack_size <1){
    return ERR_STACK_UNDERFLOW;
  }
  printf("%p\n",machine->stack[machine->stack_size-1].as_ptr);
  machine->stack_size -= 1;
  return ERR_OK;
}

err_t vm_print_i64(struct vm* machine){
  if (machine->stack_size <1){
    return ERR_STACK_UNDERFLOW;
  }
  printf("%" PRId64 "\n",machine->stack[machine->stack_size-1].as_i64);
  machine->stack_size -= 1;
  return ERR_OK;
}


err_t vm_dump_memory(struct vm* machine){
    if (machine->stack_size <2){
        return ERR_STACK_UNDERFLOW;
    }
    mem_addr  addr = machine->stack[machine->stack_size -2].as_u64;
    uint64_t count = machine->stack[machine->stack_size -1].as_u64;
    if (addr >= MAX_STATIC_MEM){
        return ERR_ILLEGAL_MEM_ACCESS;
    }
    if (addr +count <addr || addr +count  >= MAX_STATIC_MEM ){
        return ERR_ILLEGAL_MEM_ACCESS;
    }

    for (uint64_t i= 0; i <count ; ++i) {
        printf("%02X " ,machine->static_memory[addr +i]);
    }
    printf("\n");
    machine->stack_size -=2;
    return ERR_OK;
}
