//
// Created by xm on 2021/8/12.
//

#include "label.h"
#include <stdio.h>
#include <assert.h>

int label_table_resolve_label(const label_table *lt,
                              string_view name,
                              Word *output) {
    for (size_t i = 0; i < lt->label_size; ++i) {
        if (sv_eq(lt->labels[i].name, name)) {
            *output = lt->labels[i].word;
            return 1;
        }
    }
    return 0;
}

int label_table_bind_label(label_table *lt,
                           string_view name,
                           Word word) {
    assert(lt->label_size < LABEL_CAPACITY);
    Word igonred = {0};
    if (label_table_resolve_label(lt, name, &igonred)) {
        return 0;
    }
    lt->labels[lt->label_size++] = (label_t) {name, word};
    return 1;
}

void label_table_push_deferred_label(label_table *lt,
                                     string_view name,
                                     inst_addr addr
) {
    assert(lt->deferred_size < UNRESOLVED_LABEL_CAPACITY);
    lt->deferred_labels[lt->deferred_size++] = (deferred_label) {
            name,
            addr
    };
}


void print_label_table(label_table *lt) {
    printf("Labels:\n");
    for (int i = 0; i < lt->label_size; ++i) {
        printf("%.*s ->%lld\n", (int) lt->labels[i].name.count, lt->labels[i].name.data, lt->labels[i].word.as_i64);
    }

    printf("deferred Labels:\n");
    for (int i = 0; i < lt->deferred_size; ++i) {
        printf("%lld ->%.*s\n", lt->deferred_labels[i].addr, (int) lt->deferred_labels[i].name.count,
               lt->deferred_labels[i].name.data);
    }
}


void*  label_table_alloc_memory(label_table* lt,size_t size) {
    assert(lt->mem_size + size <= MAX_MEM_SIZE);
    void *result = lt->memory + size;
    lt->mem_size += size;
    return result;

}
