//
// Created by xm on 2021/8/28.
//

#ifndef VM_SRC_NATIVE_CALL_H
#define VM_SRC_NATIVE_CALL_H
#include "error_t.h"
struct vm;
typedef err_t (*native_func)(struct vm*);

err_t vm_malloc(struct vm* );
err_t vm_free(struct vm*);
err_t vm_print_f64(struct vm* );
err_t vm_print_u64(struct vm* );
err_t vm_print_ptr(struct vm* );
err_t vm_print_i64(struct vm* );
err_t vm_dump_memory(struct vm*);

void push_native_fun(struct vm*,native_func);

#endif //VM_SRC_NATIVE_CALL_H
