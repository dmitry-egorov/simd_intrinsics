#pragma once

#include "lu_assert.h"
#include "lu_prims.h"

template<typename t>
struct lu_arr {
    t* items;
    usize capacity;

    inline auto operator[](usize index) -> t& {
        lu_assert(index < capacity);
        return items[index];
    }

    inline auto zero() -> void {
        lu_assert(items);
        memset(items, 0, sizeof(t) * capacity);
    }

    inline auto fill(t* data) {
        memcpy(items, data, sizeof(t) * capacity);
    }
};

template<typename t>
static inline auto lu_malloc_arr(usize capacity) -> lu_arr<t> {
    return {
        .items = (t*)malloc(sizeof(t) * capacity),
        .capacity = capacity
    };
}

template<typename t>
static inline auto lu_aligned_alloc_arr(usize alignment, usize capacity) -> lu_arr<t> {
    return {
        //.items = (t*)_aligned_malloc(alignment, sizeof(t) * capacity),
        .items = (t*)_aligned_malloc(sizeof(t) * capacity, alignment),
        .capacity = capacity
    };
}

template<typename t>
static inline auto lu_calloc_arr(usize capacity) -> lu_arr<t> {
    return {
        .items = (t*)calloc(capacity, sizeof(t)),
        .capacity = capacity
    };
}

template<typename t>
static inline auto lu_free_arr(lu_arr<t>* arr_p) -> void {
    if (arr_p->items)
        free(arr_p->items);

    *arr_p = {};
}
