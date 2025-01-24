//
// Lanski's Utilities
//     made by Dmitry Egorov
//
#pragma once
#include <immintrin.h>
#include "lu_prims.h"

struct mm_b32x8 {
    __m256i rgx;

    inline auto operator|(mm_b32x8 other) const -> mm_b32x8 { return { _mm256_or_si256(rgx, other.rgx) }; }

    inline auto reduce() -> u8 { return (u8)_mm256_movemask_ps(rgx); }
};

struct mm_i32x8 {
    __m256i rgx;

    inline auto operator+ (mm_i32x8 other) const -> mm_i32x8 { return { _mm256_add_epi32(rgx, other.rgx) }; }
    inline auto operator+=(mm_i32x8 other) -> mm_i32x8& { rgx = _mm256_add_epi32(rgx, other.rgx); return *this; }

    inline auto hsum() const -> i32 {
        auto sum = (i32)0;
        auto sp  = (i32*)&rgx;
        for (auto i = 0; i < 8; i++)
            sum += sp[i];

        return sum;
    }
};

struct mm_u32x8 {
    __m256i rgx;

    inline auto operator+ (mm_u32x8 other) const -> mm_u32x8 { return { _mm256_add_epi32(rgx, other.rgx) }; }
    inline auto operator+=(mm_u32x8 other) -> mm_u32x8& { rgx = _mm256_add_epi32(rgx, other.rgx); return *this; }

    inline auto operator==(mm_u32x8 other) const -> mm_b32x8 { return { _mm256_cmpeq_epi32(rgx, other.rgx) }; }


    inline auto hsum() const -> u32 {
        auto sum = (u32)0;
        auto sp  = (u32*)&rgx;
        for (auto i = 0; i < 8; i++)
            sum += sp[i];

        return sum;
    }
};

static inline auto mm_u32x8_broadcast(u32 v) -> mm_u32x8 { return { _mm256_set1_epi32(v) }; }