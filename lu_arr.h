// Lanski's Utilities
//     by Dmitry Egorov
#pragma once

#include "lu_syntax.h"
#include "lu_assert.h"
#include "lu_prims.h"

lu_tt struct lu_arr {
    t* items;
    usize capacity;

    lu_mt operator[](usize index) -> t& {
        lu_assert(index < capacity);
        return items[index];
    }

    lu_mt zero() -> void {
        lu_assert(items);
        memset(items, 0, sizeof(t) * capacity);
    }

    lu_mt fill(t* data) {
        memcpy(items, data, sizeof(t) * capacity);
    }
};

lu_tt_fn lu_malloc_arr(usize capacity) -> lu_arr<t> {
    return {
        .items = (t*)malloc(sizeof(t) * capacity),
        .capacity = capacity
    };
}

lu_tt_fn lu_aligned_alloc_arr(usize alignment, usize capacity) -> lu_arr<t> {
    return {
        //.items = (t*)_aligned_malloc(alignment, sizeof(t) * capacity),
        .items = (t*)_aligned_malloc(sizeof(t) * capacity, alignment),
        .capacity = capacity
    };
}

lu_tt_fn lu_calloc_arr(usize capacity) -> lu_arr<t> {
    return {
        .items = (t*)calloc(capacity, sizeof(t)),
        .capacity = capacity
    };
}

lu_tt_fn lu_free_arr(lu_arr<t>* arr_p) -> void {
    if (arr_p->items)
        free(arr_p->items);

    *arr_p = {};
}

lu_tt_fn lu_aligned_free_arr(lu_arr<t>* arr_p) -> void {
    if (arr_p->items)
        _aligned_free(arr_p->items);

    *arr_p = {};
}
