//
// Created by xm on 2021/8/12.
//

#include "context.h"
#include "utils.h"
#include <assert.h>

bool ctx_resolve_label(const context *lt,
                              string_view name,
                              Word *output) {
    for (size_t i = 0; i < lt->label_size; ++i) {
        if (sv_eq(lt->labels[i].name, name)) {
            *output = lt->labels[i].value;
            return true;
        }
    }
    return false;
}

int ctx_bind_label(context *lt,
                           string_view name,
                           Word word) {
    assert(lt->label_size < LABEL_CAPACITY);
    Word igonred = {0};
    if (ctx_resolve_label(lt, name, &igonred)) {
        return 0;
    }
    lt->labels[lt->label_size++] = (binding) {name, word};
    return 1;
}

void ctx_push_deferred_label(context *lt,
                                     string_view name,
                                     inst_addr addr
) {
    assert(lt->deferred_size < UNRESOLVED_LABEL_CAPACITY);
    lt->deferred_labels[lt->deferred_size++] = (deferred_binding) {
            name,
            addr
    };
}


Word ctx_push_string_to_memory(context* ctx, string_view sv){
    assert(ctx->memory_size + sv.count <= MAX_MEM_SIZE );

    Word result = u64_to_word(ctx->memory_size);
    memcpy(ctx->memory + ctx->memory_size,sv.data,sv.count);
    ctx->memory_size += sv.count;

    if (ctx->memory_size > ctx->memory_capacity){
        ctx->memory_capacity = ctx->memory_size;
    }
    return result;
}

void*  ctx_alloc_memory(context * lt,size_t size) {
    assert(lt->arena_size + size <= MAX_MEM_SIZE);
    void *result = lt->arena + size;
    lt->arena_size += size;
    return result;
}
