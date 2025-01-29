#include <cstdio>

#include "lu_assert.h"
#include "lu_prims.h"
#include "lu_math.h"
#include "lu_mm.h"
#include "lu_arr.h"
#include "lu_bench.h"

int main(void) {
    static const auto  l1_size =          16 * 1024;
    static const auto  l2_size =         384 * 1024;
    static const auto  l3_size =    6 * 1024 * 1024;
    static const auto mem_size =  256 * 1024 * 1024;
    static const auto max_size = 1024 * 1024 * 1024;

    static const auto max_count_u32 = max_size / 4;

    static const usize sizes[] = {
        l1_size,
        l2_size,
        l3_size,
        mem_size,
        max_size,
    };

    printf("Estimating cpu timer frequency...\n");
    lu_init_cpu_timer(300);

    printf("Filling data buffer...\n");

    auto src_arr = lu_aligned_alloc_arr<u32>(64, max_count_u32);
    auto dst_arr = lu_aligned_alloc_arr<u32>(64, max_count_u32);

    auto src_u8 =       (u8*)src_arr.items;
    auto src_x8 = (mm_u32x8*)src_arr.items;

    { // fill
        for (usize i = 0; i < max_count_u32; i++)
            src_arr[i] = rand();
    }

    printf("Benchmarking...\n");
    for (usize size_i = 0; size_i < std::size(sizes); size_i++) {
        auto byte_size = sizes[size_i];
        auto count_u32   = byte_size / 4;
        auto count_u32x8 = count_u32 / 8;


        char size_text_buf[16];
        snprintf(size_text_buf, 16, "%lluk", byte_size / 1024);
        printf("data size: %-8s              min                          avg\n", size_text_buf);

        lu_benchmark("baseline read 32x4", byte_size, [&] {
            __asm {
                mov rax, src_u8
                mov rcx, byte_size
                loop:
                    vmovdqu ymm0, [rax + 0x00]
                    vmovdqu ymm0, [rax + 0x20]
                    vmovdqu ymm0, [rax + 0x40]
                    vmovdqu ymm0, [rax + 0x60]

                    add rax, 0x80
                    sub rcx, 0x80
                jnz loop
            }

            return true;
        }, [&](bool result) { return result; });

        lu_benchmark("baseline read 32x8", byte_size, [&] {
            __asm {
                mov rax, src_u8
                mov rcx, byte_size
                loop:
                    vmovdqu ymm0, [rax + 0x00]
                    vmovdqu ymm0, [rax + 0x20]
                    vmovdqu ymm0, [rax + 0x40]
                    vmovdqu ymm0, [rax + 0x60]
                    vmovdqu ymm0, [rax + 0x80]
                    vmovdqu ymm0, [rax + 0xa0]
                    vmovdqu ymm0, [rax + 0xc0]
                    vmovdqu ymm0, [rax + 0xe0]

                    add rax, 0x100
                    sub rcx, 0x100
                jnz loop
            }

            return true;
        }, [&](bool result) { return result; });

        lu_benchmark("baseline read 32x16", byte_size, [&] {
            __asm {
                mov rax, src_u8
                mov rcx, byte_size
                loop:
                    vmovdqu ymm0, [rax + 0x000]
                    vmovdqu ymm0, [rax + 0x020]
                    vmovdqu ymm0, [rax + 0x040]
                    vmovdqu ymm0, [rax + 0x060]
                    vmovdqu ymm0, [rax + 0x080]
                    vmovdqu ymm0, [rax + 0x0a0]
                    vmovdqu ymm0, [rax + 0x0c0]
                    vmovdqu ymm0, [rax + 0x0e0]
                    vmovdqu ymm0, [rax + 0x100]
                    vmovdqu ymm0, [rax + 0x120]
                    vmovdqu ymm0, [rax + 0x140]
                    vmovdqu ymm0, [rax + 0x160]
                    vmovdqu ymm0, [rax + 0x180]
                    vmovdqu ymm0, [rax + 0x1a0]
                    vmovdqu ymm0, [rax + 0x1c0]
                    vmovdqu ymm0, [rax + 0x1e0]

                    add rax, 0x200
                    sub rcx, 0x200
                jnz loop
            }

            return true;
        }, [&](bool result) { return result; });

        auto sum_result = (u32)0;

        // calculate the actual sum
        {
            for (usize i = 0; i < count_u32; i++)
                sum_result += src_arr[i];
        }

        //if (0)
        lu_benchmark("sum simple", byte_size, [&] {
            auto sum = (u32)0;
            for (usize i = 0; i < count_u32; i++)
                sum += src_arr[i];
            return sum;
        }, [&](u32 result) { return result == sum_result; });

        //if (0)
        lu_benchmark("sum simd x2", byte_size, [&] {
            mm_u32x8 s0 = {}, s1 = {};

            for (usize i = 0; i < count_u32x8; i += 2) {
                s0 += src_x8[i + 0];
                s1 += src_x8[i + 1];
            }

            return (s0 + s1).hsum();
        }, [&](u32 result) { return result == sum_result; });

        //if (0)
        lu_benchmark("sum simd x4", byte_size, [&] {
            mm_u32x8 s0 = {}, s1 = {}, s2 = {}, s3 = {};

            for (usize i = 0; i < count_u32x8; i += 4) {
                s0 += src_x8[i + 0];
                s1 += src_x8[i + 1];
                s2 += src_x8[i + 2];
                s3 += src_x8[i + 3];
            }

            return ((s0 + s1) + (s2 + s3)).hsum();
        }, [&](u32 result) { return result == sum_result; });

        //if (0)
        lu_benchmark("sum simd x8", byte_size, [&] {
            mm_u32x8 s0 = {}, s1 = {}, s2 = {}, s3 = {}, s4 = {}, s5 = {}, s6 = {}, s7 = {};

            for (usize i = 0; i < count_u32x8; i += 8) {
                s0 += src_x8[i + 0];
                s1 += src_x8[i + 1];
                s2 += src_x8[i + 2];
                s3 += src_x8[i + 3];
                s4 += src_x8[i + 4];
                s5 += src_x8[i + 5];
                s6 += src_x8[i + 6];
                s7 += src_x8[i + 7];
            }

            return (((s0 + s1) + (s2 + s3)) + ((s4 + s5) + (s6 + s7))).hsum();
        }, [&](u32 result) { return result == sum_result; });

        //if (0)
        lu_benchmark("sum simd x16", byte_size, [&] {
            mm_u32x8 s0 = {}, s1 = {}, s2 = {}, s3 = {}, s4 = {}, s5 = {}, s6 = {}, s7 = {};
            mm_u32x8 s8 = {}, s9 = {}, s10 = {}, s11 = {}, s12 = {}, s13 = {}, s14 = {}, s15 = {};

            for (usize i = 0; i < count_u32x8; i += 16) {
                s0  += src_x8[i +  0];
                s1  += src_x8[i +  1];
                s2  += src_x8[i +  2];
                s3  += src_x8[i +  3];
                s4  += src_x8[i +  4];
                s5  += src_x8[i +  5];
                s6  += src_x8[i +  6];
                s7  += src_x8[i +  7];
                s8  += src_x8[i +  8];
                s9  += src_x8[i +  9];
                s10 += src_x8[i + 10];
                s11 += src_x8[i + 11];
                s12 += src_x8[i + 12];
                s13 += src_x8[i + 13];
                s14 += src_x8[i + 14];
                s15 += src_x8[i + 15];
            }

            return (
                ((s0 + s1) + (s2  + s3 )) + ((s4  + s5 ) + (s6  + s7 )) +
                ((s8 + s9) + (s10 + s11)) + ((s12 + s13) + (s14 + s15))
            ).hsum();
        }, [&](u32 result) { return result == sum_result; });

        //if (0)
        lu_benchmark("simple swap", byte_size, [&] {
            for (usize i = 0; i < count_u32; i++)
                dst_arr[count_u32 - i - 1] = src_arr[i];

            return 0;
        }, [&](u32) {
            for (usize i = 0; i < count_u32; i++)
                if (dst_arr[count_u32 - i - 1] != src_arr[i])
                    return false;

            return true;
        });

        auto last_item = src_arr[count_u32 - 1];

        { // remove duplicates of the last item
            for(usize i = 0; i < count_u32 - 1; i++)
                if (src_arr[i] == last_item)
                    src_arr[i] = 1111;
        }

        //if (0)
        lu_benchmark("find simple", byte_size, [&] {
            for (usize i = 0; i < count_u32; i++)
                if (src_arr[i] == last_item)
                    return i;

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simple x2", byte_size, [&] {
            for (usize i = 0; i < count_u32; i += 2) {
                auto
                    m0 = src_arr[i + 0] == last_item,
                    m1 = src_arr[i + 1] == last_item
                ;

                if (m0 | m1)
                    return m0 ? i : i + 1;
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simple x4", byte_size, [&] {
            for (usize i = 0; i < count_u32; i += 4) {
                auto
                    m0 = src_arr[i + 0] == last_item,
                    m1 = src_arr[i + 1] == last_item,
                    m2 = src_arr[i + 2] == last_item,
                    m3 = src_arr[i + 3] == last_item
                ;

                if (m0 | m1 | m2 | m3)
                    return m0 ? i : m1 ? i + 1 : m2 ? i + 2 : i + 3;
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simple x8", byte_size, [&] {
            for (usize i = 0; i < count_u32; i += 8) {
                auto
                    m0 = src_arr[i + 0] == last_item,
                    m1 = src_arr[i + 1] == last_item,
                    m2 = src_arr[i + 2] == last_item,
                    m3 = src_arr[i + 3] == last_item,
                    m4 = src_arr[i + 4] == last_item,
                    m5 = src_arr[i + 5] == last_item,
                    m6 = src_arr[i + 6] == last_item,
                    m7 = src_arr[i + 7] == last_item
                ;

                if (((m0 | m1) | (m2 | m3)) | ((m4 | m5) | (m6 | m7)))
                    return
                          m0 ? i + 0
                        : m1 ? i + 1
                        : m2 ? i + 2
                        : m3 ? i + 3
                        : m4 ? i + 4
                        : m5 ? i + 5
                        : m6 ? i + 6
                        :      i + 7;
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simd", byte_size, [&] {
            auto target_x8 = mm_u32x8_broadcast(last_item);

            for (usize i = 0; i < count_u32x8; i += 1) {
                auto m_x8 = target_x8 == src_x8[i];
                if (auto mask = m_x8.reduce())
                    return i * 8 + __builtin_ctz(mask);
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simd 2", byte_size, [&] {
            auto target_x8 = _mm256_set1_epi32(last_item);

            for (usize i = 0; i < count_u32; i += 8) {
                auto m_x8 = _mm256_cmpeq_epi32(target_x8, *(__m256i*)&src_arr.items[i]);
                if (!_mm256_testz_si256(m_x8, m_x8))
                    return i + __builtin_ctz(_mm256_movemask_ps(m_x8));
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simd x2", byte_size, [&] {
            auto target_x8 = mm_u32x8_broadcast(last_item);

            for (usize i = 0; i < count_u32x8; i += 2) {
                auto
                    m_x8_0 = target_x8 == src_x8[i + 0],
                    m_x8_1 = target_x8 == src_x8[i + 1]
                ;

                if ((m_x8_0 | m_x8_1).reduce()) {
                    if (auto m_0 = m_x8_0.reduce()) return (i + 0) * 8 + __builtin_ctz(m_0);
                    if (auto m_1 = m_x8_1.reduce()) return (i + 1) * 8 + __builtin_ctz(m_1);

                    lu_assert(false);
                    return (usize) -1;
                }
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simd x4", byte_size, [&] {
            auto target_x8 = mm_u32x8_broadcast(last_item);

            for (usize i = 0; i < count_u32x8; i += 4) {
                auto
                    m_x8_0 = target_x8 == src_x8[i + 0],
                    m_x8_1 = target_x8 == src_x8[i + 1],
                    m_x8_2 = target_x8 == src_x8[i + 2],
                    m_x8_3 = target_x8 == src_x8[i + 3]
                ;

                if (((m_x8_0 | m_x8_1) | (m_x8_2 | m_x8_3)).reduce()) {
                    if (auto m_0 = m_x8_0.reduce()) return (i + 0) * 8 + __builtin_ctz(m_0);
                    if (auto m_1 = m_x8_1.reduce()) return (i + 1) * 8 + __builtin_ctz(m_1);
                    if (auto m_2 = m_x8_2.reduce()) return (i + 2) * 8 + __builtin_ctz(m_2);
                    if (auto m_3 = m_x8_3.reduce()) return (i + 3) * 8 + __builtin_ctz(m_3);

                    lu_assert(false);
                    return (usize) -1;
                }
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simd x8", byte_size, [&] {
            auto target_x8 = mm_u32x8_broadcast(last_item);

            for (usize i = 0; i < count_u32x8; i += 8) {
                auto
                    m_x8_0 = target_x8 == src_x8[i + 0],
                    m_x8_1 = target_x8 == src_x8[i + 1],
                    m_x8_2 = target_x8 == src_x8[i + 2],
                    m_x8_3 = target_x8 == src_x8[i + 3],
                    m_x8_4 = target_x8 == src_x8[i + 4],
                    m_x8_5 = target_x8 == src_x8[i + 5],
                    m_x8_6 = target_x8 == src_x8[i + 6],
                    m_x8_7 = target_x8 == src_x8[i + 7]
                ;

                if ((((m_x8_0 | m_x8_1) | (m_x8_2 | m_x8_3)) | ((m_x8_4 | m_x8_5) | (m_x8_6 | m_x8_7))).reduce()) {
                    if (auto m_0 = m_x8_0.reduce()) return (i + 0) * 8 + __builtin_ctz(m_0);
                    if (auto m_1 = m_x8_1.reduce()) return (i + 1) * 8 + __builtin_ctz(m_1);
                    if (auto m_2 = m_x8_2.reduce()) return (i + 2) * 8 + __builtin_ctz(m_2);
                    if (auto m_3 = m_x8_3.reduce()) return (i + 3) * 8 + __builtin_ctz(m_3);
                    if (auto m_4 = m_x8_4.reduce()) return (i + 4) * 8 + __builtin_ctz(m_4);
                    if (auto m_5 = m_x8_5.reduce()) return (i + 5) * 8 + __builtin_ctz(m_5);
                    if (auto m_6 = m_x8_6.reduce()) return (i + 6) * 8 + __builtin_ctz(m_6);
                    if (auto m_7 = m_x8_7.reduce()) return (i + 7) * 8 + __builtin_ctz(m_7);

                    lu_assert(false);
                    return (usize) -1;
                }
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        //if (0)
        lu_benchmark("find simd x16", byte_size, [&] {
            auto target_x8 = mm_u32x8_broadcast(last_item);

            for (usize i = 0; i < count_u32x8; i += 16) {
                auto
                    m_x8_0  = target_x8 == src_x8[i +  0],
                    m_x8_1  = target_x8 == src_x8[i +  1],
                    m_x8_2  = target_x8 == src_x8[i +  2],
                    m_x8_3  = target_x8 == src_x8[i +  3],
                    m_x8_4  = target_x8 == src_x8[i +  4],
                    m_x8_5  = target_x8 == src_x8[i +  5],
                    m_x8_6  = target_x8 == src_x8[i +  6],
                    m_x8_7  = target_x8 == src_x8[i +  7],
                    m_x8_8  = target_x8 == src_x8[i +  8],
                    m_x8_9  = target_x8 == src_x8[i +  9],
                    m_x8_10 = target_x8 == src_x8[i + 10],
                    m_x8_11 = target_x8 == src_x8[i + 11],
                    m_x8_12 = target_x8 == src_x8[i + 12],
                    m_x8_13 = target_x8 == src_x8[i + 13],
                    m_x8_14 = target_x8 == src_x8[i + 14],
                    m_x8_15 = target_x8 == src_x8[i + 15]
                ;

                auto m_x8_all =
                      (((m_x8_0 | m_x8_1) | (m_x8_2  | m_x8_3 )) | ((m_x8_4  | m_x8_5 ) | (m_x8_6  | m_x8_7 )))
                    | (((m_x8_8 | m_x8_9) | (m_x8_10 | m_x8_11)) | ((m_x8_12 | m_x8_13) | (m_x8_14 | m_x8_15)));

                if (m_x8_all.reduce()) {
                    if (auto m_0  = m_x8_0 .reduce()) return (i +  0) * 8 + __builtin_ctz(m_0 );
                    if (auto m_1  = m_x8_1 .reduce()) return (i +  1) * 8 + __builtin_ctz(m_1 );
                    if (auto m_2  = m_x8_2 .reduce()) return (i +  2) * 8 + __builtin_ctz(m_2 );
                    if (auto m_3  = m_x8_3 .reduce()) return (i +  3) * 8 + __builtin_ctz(m_3 );
                    if (auto m_4  = m_x8_4 .reduce()) return (i +  4) * 8 + __builtin_ctz(m_4 );
                    if (auto m_5  = m_x8_5 .reduce()) return (i +  5) * 8 + __builtin_ctz(m_5 );
                    if (auto m_6  = m_x8_6 .reduce()) return (i +  6) * 8 + __builtin_ctz(m_6 );
                    if (auto m_7  = m_x8_7 .reduce()) return (i +  7) * 8 + __builtin_ctz(m_7 );
                    if (auto m_8  = m_x8_8 .reduce()) return (i +  8) * 8 + __builtin_ctz(m_8 );
                    if (auto m_9  = m_x8_9 .reduce()) return (i +  9) * 8 + __builtin_ctz(m_9 );
                    if (auto m_10 = m_x8_10.reduce()) return (i + 10) * 8 + __builtin_ctz(m_10);
                    if (auto m_11 = m_x8_11.reduce()) return (i + 11) * 8 + __builtin_ctz(m_11);
                    if (auto m_12 = m_x8_12.reduce()) return (i + 12) * 8 + __builtin_ctz(m_12);
                    if (auto m_13 = m_x8_13.reduce()) return (i + 13) * 8 + __builtin_ctz(m_13);
                    if (auto m_14 = m_x8_14.reduce()) return (i + 14) * 8 + __builtin_ctz(m_14);
                    if (auto m_15 = m_x8_15.reduce()) return (i + 15) * 8 + __builtin_ctz(m_15);

                    lu_assert(false);
                    return (usize) -1;
                }
            }

            return (usize)-1;
        }, [&](usize idx) { return (idx != (usize)-1) && (src_arr[idx] == last_item); });

        auto min_actual = (u32)-1;
        for (usize i = 0; i < count_u32; i++) {
            if (src_arr[i] < min_actual)
                min_actual = src_arr[i];
        }

        //if (0)
        lu_benchmark("min simple", byte_size, [&] {
            auto min = (u32)-1;
            for (usize i = 0; i < count_u32; i++)
                min = lu_min(src_arr[i], min);

            return min;
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simple x2", byte_size, [&] {
            auto min_0 = (u32)-1;
            auto min_1 = (u32)-1;
            for (usize i = 0; i < count_u32; i += 2) {
                min_0 = lu_min(src_arr[i + 0], min_0);
                min_1 = lu_min(src_arr[i + 1], min_1);
            }

            return lu_min(min_0, min_1);
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simple x4", byte_size, [&] {
            auto min_0 = (u32)-1;
            auto min_1 = (u32)-1;
            auto min_2 = (u32)-1;
            auto min_3 = (u32)-1;
            for (usize i = 0; i < count_u32; i += 4) {
                min_0 = lu_min(src_arr[i + 0], min_0);
                min_1 = lu_min(src_arr[i + 1], min_1);
                min_2 = lu_min(src_arr[i + 2], min_2);
                min_3 = lu_min(src_arr[i + 3], min_3);
            }

            return lu_min(lu_min(min_0, min_1), lu_min(min_2, min_3));
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simple x8", byte_size, [&] {
            auto min_0 = (u32)-1;
            auto min_1 = (u32)-1;
            auto min_2 = (u32)-1;
            auto min_3 = (u32)-1;
            auto min_4 = (u32)-1;
            auto min_5 = (u32)-1;
            auto min_6 = (u32)-1;
            auto min_7 = (u32)-1;
            for (usize i = 0; i < count_u32; i += 8) {
                min_0 = lu_min(src_arr[i + 0], min_0);
                min_1 = lu_min(src_arr[i + 1], min_1);
                min_2 = lu_min(src_arr[i + 2], min_2);
                min_3 = lu_min(src_arr[i + 3], min_3);
                min_4 = lu_min(src_arr[i + 4], min_4);
                min_5 = lu_min(src_arr[i + 5], min_5);
                min_6 = lu_min(src_arr[i + 6], min_6);
                min_7 = lu_min(src_arr[i + 7], min_7);
            }

            return lu_min(
                lu_min(lu_min(min_0, min_1), lu_min(min_2, min_3)),
                lu_min(lu_min(min_4, min_5), lu_min(min_6, min_7))
            );
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simple x16", byte_size, [&] {
            auto min_0  = (u32)-1;
            auto min_1  = (u32)-1;
            auto min_2  = (u32)-1;
            auto min_3  = (u32)-1;
            auto min_4  = (u32)-1;
            auto min_5  = (u32)-1;
            auto min_6  = (u32)-1;
            auto min_7  = (u32)-1;
            auto min_8  = (u32)-1;
            auto min_9  = (u32)-1;
            auto min_10 = (u32)-1;
            auto min_11 = (u32)-1;
            auto min_12 = (u32)-1;
            auto min_13 = (u32)-1;
            auto min_14 = (u32)-1;
            auto min_15 = (u32)-1;
            for (usize i = 0; i < count_u32; i += 16) {
                min_0  = lu_min(src_arr[i +  0], min_0 );
                min_1  = lu_min(src_arr[i +  1], min_1 );
                min_2  = lu_min(src_arr[i +  2], min_2 );
                min_3  = lu_min(src_arr[i +  3], min_3 );
                min_4  = lu_min(src_arr[i +  4], min_4 );
                min_5  = lu_min(src_arr[i +  5], min_5 );
                min_6  = lu_min(src_arr[i +  6], min_6 );
                min_7  = lu_min(src_arr[i +  7], min_7 );
                min_8  = lu_min(src_arr[i +  8], min_8 );
                min_9  = lu_min(src_arr[i +  9], min_9 );
                min_10 = lu_min(src_arr[i + 10], min_10);
                min_11 = lu_min(src_arr[i + 11], min_11);
                min_12 = lu_min(src_arr[i + 12], min_12);
                min_13 = lu_min(src_arr[i + 13], min_13);
                min_14 = lu_min(src_arr[i + 14], min_14);
                min_15 = lu_min(src_arr[i + 15], min_15);
            }

            return lu_min(
                lu_min(
                    lu_min(lu_min(min_0, min_1), lu_min(min_2, min_3)),
                    lu_min(lu_min(min_4, min_5), lu_min(min_6, min_7))
                ),
                lu_min(
                    lu_min(lu_min(min_8 , min_9 ), lu_min(min_10, min_11)),
                    lu_min(lu_min(min_12, min_13), lu_min(min_14, min_15))
                )
            );
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simd", byte_size, [&] {
            auto min_x8 = mm_u32x8_broadcast(-1);

            for (usize i = 0; i < count_u32x8; i++)
                min_x8 = mm_min(min_x8, src_x8[i +  0]);

            return min_x8.hmin();
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simd x2", byte_size, [&] {
            auto min_x8_0 = mm_u32x8_broadcast(-1);
            auto min_x8_1 = mm_u32x8_broadcast(-1);

            for (usize i = 0; i < count_u32x8; i += 2) {
                min_x8_0 = mm_min(min_x8_0, src_x8[i + 0]);
                min_x8_1 = mm_min(min_x8_1, src_x8[i + 1]);
            }

            return mm_min(min_x8_0, min_x8_1).hmin();
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simd x4", byte_size, [&] {
            auto min_x8_0 = mm_u32x8_broadcast(-1);
            auto min_x8_1 = mm_u32x8_broadcast(-1);
            auto min_x8_2 = mm_u32x8_broadcast(-1);
            auto min_x8_3 = mm_u32x8_broadcast(-1);

            for (usize i = 0; i < count_u32x8; i += 4) {
                min_x8_0 = mm_min(min_x8_0, src_x8[i + 0]);
                min_x8_1 = mm_min(min_x8_1, src_x8[i + 1]);
                min_x8_2 = mm_min(min_x8_2, src_x8[i + 2]);
                min_x8_3 = mm_min(min_x8_3, src_x8[i + 3]);
            }

            return mm_min(mm_min(min_x8_0, min_x8_1), mm_min(min_x8_2, min_x8_3)).hmin();
        }, [&](u32 result) { return result == min_actual; });

        ////if (0)
        lu_benchmark("min simd x8", byte_size, [&] {
            auto min_x8_0 = mm_u32x8_broadcast(-1);
            auto min_x8_1 = mm_u32x8_broadcast(-1);
            auto min_x8_2 = mm_u32x8_broadcast(-1);
            auto min_x8_3 = mm_u32x8_broadcast(-1);
            auto min_x8_4 = mm_u32x8_broadcast(-1);
            auto min_x8_5 = mm_u32x8_broadcast(-1);
            auto min_x8_6 = mm_u32x8_broadcast(-1);
            auto min_x8_7 = mm_u32x8_broadcast(-1);

            for (usize i = 0; i < count_u32x8; i += 8) {
                min_x8_0 = mm_min(min_x8_0, src_x8[i + 0]);
                min_x8_1 = mm_min(min_x8_1, src_x8[i + 1]);
                min_x8_2 = mm_min(min_x8_2, src_x8[i + 2]);
                min_x8_3 = mm_min(min_x8_3, src_x8[i + 3]);
                min_x8_4 = mm_min(min_x8_4, src_x8[i + 4]);
                min_x8_5 = mm_min(min_x8_5, src_x8[i + 5]);
                min_x8_6 = mm_min(min_x8_6, src_x8[i + 6]);
                min_x8_7 = mm_min(min_x8_7, src_x8[i + 7]);
            }

            return mm_min(
                mm_min(mm_min(min_x8_0, min_x8_1), mm_min(min_x8_2, min_x8_3)),
                mm_min(mm_min(min_x8_4, min_x8_5), mm_min(min_x8_6, min_x8_7))
            ).hmin();
        }, [&](u32 result) { return result == min_actual; });

        //if (0)
        lu_benchmark("min simd x16", byte_size, [&] {
            auto min_x8_0  = mm_u32x8_broadcast(-1);
            auto min_x8_1  = mm_u32x8_broadcast(-1);
            auto min_x8_2  = mm_u32x8_broadcast(-1);
            auto min_x8_3  = mm_u32x8_broadcast(-1);
            auto min_x8_4  = mm_u32x8_broadcast(-1);
            auto min_x8_5  = mm_u32x8_broadcast(-1);
            auto min_x8_6  = mm_u32x8_broadcast(-1);
            auto min_x8_7  = mm_u32x8_broadcast(-1);
            auto min_x8_8  = mm_u32x8_broadcast(-1);
            auto min_x8_9  = mm_u32x8_broadcast(-1);
            auto min_x8_10 = mm_u32x8_broadcast(-1);
            auto min_x8_11 = mm_u32x8_broadcast(-1);
            auto min_x8_12 = mm_u32x8_broadcast(-1);
            auto min_x8_13 = mm_u32x8_broadcast(-1);
            auto min_x8_14 = mm_u32x8_broadcast(-1);
            auto min_x8_15 = mm_u32x8_broadcast(-1);

            for (usize i = 0; i < count_u32x8; i += 16) {
                min_x8_0  = mm_min(min_x8_0 , src_x8[i +  0]);
                min_x8_1  = mm_min(min_x8_1 , src_x8[i +  1]);
                min_x8_2  = mm_min(min_x8_2 , src_x8[i +  2]);
                min_x8_3  = mm_min(min_x8_3 , src_x8[i +  3]);
                min_x8_4  = mm_min(min_x8_4 , src_x8[i +  4]);
                min_x8_5  = mm_min(min_x8_5 , src_x8[i +  5]);
                min_x8_6  = mm_min(min_x8_6 , src_x8[i +  6]);
                min_x8_7  = mm_min(min_x8_7 , src_x8[i +  7]);
                min_x8_8  = mm_min(min_x8_8 , src_x8[i +  8]);
                min_x8_9  = mm_min(min_x8_9 , src_x8[i +  9]);
                min_x8_10 = mm_min(min_x8_10, src_x8[i + 10]);
                min_x8_11 = mm_min(min_x8_11, src_x8[i + 11]);
                min_x8_12 = mm_min(min_x8_12, src_x8[i + 12]);
                min_x8_13 = mm_min(min_x8_13, src_x8[i + 13]);
                min_x8_14 = mm_min(min_x8_14, src_x8[i + 14]);
                min_x8_15 = mm_min(min_x8_15, src_x8[i + 15]);
            }

            return mm_min(
                mm_min(
                    mm_min(mm_min(min_x8_0, min_x8_1), mm_min(min_x8_2, min_x8_3)),
                    mm_min(mm_min(min_x8_4, min_x8_5), mm_min(min_x8_6, min_x8_7))
                ),
                mm_min(
                    mm_min(mm_min(min_x8_8 , min_x8_9 ), mm_min(min_x8_10, min_x8_11)),
                    mm_min(mm_min(min_x8_12, min_x8_13), mm_min(min_x8_14, min_x8_15))
                )
            ).hmin();
        }, [&](u32 result) { return result == min_actual; });

        auto min_idx_actual = src_arr[0];
        {
            auto min   = src_arr[0];
            auto min_i = 0;
            for (usize i = 0; i < count_u32; i++) {
                if (src_arr[i] < min) {
                    min = src_arr[i];
                    min_i = i;
                }
            }

            min_idx_actual = min_i;
        }

        lu_benchmark("min idx simple", byte_size, [&] {
            auto min   = src_arr[0];
            auto min_i = 0;
            for (usize i = 0; i < count_u32; i++) {
                if (src_arr[i] < min) {
                    min = src_arr[i];
                    min_i = i;
                }
            }

            return min_i;
        }, [&](u32 result) { return result == min_idx_actual; });

        static const auto block_size_u32   = 256;
        static const auto block_size_u32x8 = block_size_u32 / 8;
        auto count_blocks = count_u32 / block_size_u32;

        lu_benchmark("min idx simd", byte_size, [&] {
            auto min         = src_arr[0];
            auto min_block_i = (usize)0;
            auto max_u32_x8  = mm_u32x8_broadcast(-1);

            // find block with min value
            for (usize bi = 0; bi < count_blocks; bi += 1) {
                auto min_x8 = max_u32_x8;

                auto first =  bi      * block_size_u32x8;
                auto last  = (bi + 1) * block_size_u32x8;
                for (auto i = first; i < last; i += 1)
                    min_x8 = mm_min(min_x8, src_x8[i]);

                auto block_min = min_x8.hmin();

                if (block_min < min) {
                    min = block_min;
                    min_block_i = bi;
                }
            }

            // find idx within the block
            auto idx = (usize)-1;
            {
                auto target_x8 = mm_u32x8_broadcast(min);
                auto first =  min_block_i      * block_size_u32x8;
                auto last  = (min_block_i + 1) * block_size_u32x8;

                for (usize ix8 = first; ix8 < last; ix8 += 1) {
                    auto m_x8 = target_x8 == src_x8[ix8];
                    if (auto mask = m_x8.reduce()) {
                        idx = ix8 * 8 + __builtin_ctz(mask);
                        break;
                    }
                }
            }

            return idx;
        }, [&](u32 result) { return result == min_idx_actual; });

        lu_benchmark("min idx simd x2", byte_size, [&] {
            auto min         = src_arr[0];
            auto min_block_i = (usize)0;
            auto max_u32_x8  = mm_u32x8_broadcast(-1);

            // find block with min value
            for (usize bi = 0; bi < count_blocks; bi += 1) {
                auto min_x8_0 = max_u32_x8;
                auto min_x8_1 = max_u32_x8;

                auto first =  bi      * block_size_u32x8;
                auto last  = (bi + 1) * block_size_u32x8;
                for (auto ix8 = first; ix8 < last; ix8 += 2) {
                    min_x8_0 = mm_min(min_x8_0, src_x8[ix8 + 0]);
                    min_x8_1 = mm_min(min_x8_1, src_x8[ix8 + 1]);
                }

                auto block_min = mm_min(min_x8_0, min_x8_1).hmin();

                if (block_min < min) {
                    min = block_min;
                    min_block_i = bi;
                }
            }

            // find idx within the block
            auto idx = (usize)-1;
            {
                auto target_x8 = mm_u32x8_broadcast(min);
                auto first =  min_block_i      * block_size_u32x8;
                auto last  = (min_block_i + 1) * block_size_u32x8;

                for (usize ix8 = first; ix8 < last; ix8 += 2) {
                    auto
                        m_x8_0 = target_x8 == src_x8[ix8 + 0],
                        m_x8_1 = target_x8 == src_x8[ix8 + 1]
                    ;

                    if ((m_x8_0 | m_x8_1).reduce()) {
                        if (auto m_0 = m_x8_0.reduce()) return (ix8 + 0) * 8 + __builtin_ctz(m_0);
                        if (auto m_1 = m_x8_1.reduce()) return (ix8 + 1) * 8 + __builtin_ctz(m_1);

                        lu_assert(false);
                        return (usize) -1;
                    }
                }
            }

            return idx;
        }, [&](u32 result) { return result == min_idx_actual; });

        lu_benchmark("min idx simd x4", byte_size, [&] {
            auto min         = src_arr[0];
            auto min_block_i = (usize)0;
            auto max_u32_x8  = mm_u32x8_broadcast(-1);

            // find block with min value
            for (usize bi = 0; bi < count_blocks; bi += 1) {
                auto min_x8_0 = max_u32_x8;
                auto min_x8_1 = max_u32_x8;
                auto min_x8_2 = max_u32_x8;
                auto min_x8_3 = max_u32_x8;

                auto first =  bi      * block_size_u32x8;
                auto last  = (bi + 1) * block_size_u32x8;
                for (auto ix8 = first; ix8 < last; ix8 += 4) {
                    min_x8_0 = mm_min(min_x8_0, src_x8[ix8 + 0]);
                    min_x8_1 = mm_min(min_x8_1, src_x8[ix8 + 1]);
                    min_x8_2 = mm_min(min_x8_2, src_x8[ix8 + 2]);
                    min_x8_3 = mm_min(min_x8_3, src_x8[ix8 + 3]);
                }

                auto block_min = mm_min(
                    mm_min(min_x8_0, min_x8_1),
                    mm_min(min_x8_2, min_x8_3)
                ).hmin();

                if (block_min < min) {
                    min = block_min;
                    min_block_i = bi;
                }
            }

            // find idx within the block
            auto idx = (usize)-1;
            {
                auto target_x8 = mm_u32x8_broadcast(min);
                auto first =  min_block_i      * block_size_u32x8;
                auto last  = (min_block_i + 1) * block_size_u32x8;

                for (usize ix8 = first; ix8 < last; ix8 += 4) {
                    auto
                        m_x8_0 = target_x8 == src_x8[ix8 + 0],
                        m_x8_1 = target_x8 == src_x8[ix8 + 1],
                        m_x8_2 = target_x8 == src_x8[ix8 + 2],
                        m_x8_3 = target_x8 == src_x8[ix8 + 3]
                    ;

                    if (((m_x8_0 | m_x8_1) | (m_x8_2 | m_x8_3)).reduce()) {
                        if (auto m_0 = m_x8_0.reduce()) return (ix8 + 0) * 8 + __builtin_ctz(m_0);
                        if (auto m_1 = m_x8_1.reduce()) return (ix8 + 1) * 8 + __builtin_ctz(m_1);
                        if (auto m_2 = m_x8_2.reduce()) return (ix8 + 2) * 8 + __builtin_ctz(m_2);
                        if (auto m_3 = m_x8_3.reduce()) return (ix8 + 3) * 8 + __builtin_ctz(m_3);

                        lu_assert(false);
                        return (usize) -1;
                    }
                }
            }

            return idx;
        }, [&](u32 result) { return result == min_idx_actual; });

        lu_benchmark("min idx simd x8", byte_size, [&] {
            auto min         = src_arr[0];
            auto min_block_i = (usize)0;
            auto max_u32_x8  = mm_u32x8_broadcast(-1);

            // find block with min value
            for (usize bi = 0; bi < count_blocks; bi += 1) {
                auto min_x8_0 = max_u32_x8;
                auto min_x8_1 = max_u32_x8;
                auto min_x8_2 = max_u32_x8;
                auto min_x8_3 = max_u32_x8;
                auto min_x8_4 = max_u32_x8;
                auto min_x8_5 = max_u32_x8;
                auto min_x8_6 = max_u32_x8;
                auto min_x8_7 = max_u32_x8;

                auto first =  bi      * block_size_u32x8;
                auto last  = (bi + 1) * block_size_u32x8;
                for (auto ix8 = first; ix8 < last; ix8 += 8) {
                    min_x8_0 = mm_min(min_x8_0, src_x8[ix8 + 0]);
                    min_x8_1 = mm_min(min_x8_1, src_x8[ix8 + 1]);
                    min_x8_2 = mm_min(min_x8_2, src_x8[ix8 + 2]);
                    min_x8_3 = mm_min(min_x8_3, src_x8[ix8 + 3]);
                    min_x8_4 = mm_min(min_x8_4, src_x8[ix8 + 4]);
                    min_x8_5 = mm_min(min_x8_5, src_x8[ix8 + 5]);
                    min_x8_6 = mm_min(min_x8_6, src_x8[ix8 + 6]);
                    min_x8_7 = mm_min(min_x8_7, src_x8[ix8 + 7]);
                }

                auto block_min = mm_min(
                    mm_min(mm_min(min_x8_0, min_x8_1), mm_min(min_x8_2, min_x8_3)),
                    mm_min(mm_min(min_x8_4, min_x8_5), mm_min(min_x8_6, min_x8_7))
                ).hmin();

                if (block_min < min) {
                    min = block_min;
                    min_block_i = bi;
                }
            }

            // find idx within the block
            auto idx = (usize)-1;
            {
                auto target_x8 = mm_u32x8_broadcast(min);
                auto first =  min_block_i      * block_size_u32x8;
                auto last  = (min_block_i + 1) * block_size_u32x8;

                for (usize ix8 = first; ix8 < last; ix8 += 8) {
                    auto
                        m_x8_0 = target_x8 == src_x8[ix8 + 0],
                        m_x8_1 = target_x8 == src_x8[ix8 + 1],
                        m_x8_2 = target_x8 == src_x8[ix8 + 2],
                        m_x8_3 = target_x8 == src_x8[ix8 + 3],
                        m_x8_4 = target_x8 == src_x8[ix8 + 4],
                        m_x8_5 = target_x8 == src_x8[ix8 + 5],
                        m_x8_6 = target_x8 == src_x8[ix8 + 6],
                        m_x8_7 = target_x8 == src_x8[ix8 + 7]
                    ;

                    if ((((m_x8_0 | m_x8_1) | (m_x8_2 | m_x8_3)) | ((m_x8_4 | m_x8_5) | (m_x8_6 | m_x8_7))).reduce()) {
                        if (auto m_0 = m_x8_0.reduce()) return (ix8 + 0) * 8 + __builtin_ctz(m_0);
                        if (auto m_1 = m_x8_1.reduce()) return (ix8 + 1) * 8 + __builtin_ctz(m_1);
                        if (auto m_2 = m_x8_2.reduce()) return (ix8 + 2) * 8 + __builtin_ctz(m_2);
                        if (auto m_3 = m_x8_3.reduce()) return (ix8 + 3) * 8 + __builtin_ctz(m_3);
                        if (auto m_4 = m_x8_4.reduce()) return (ix8 + 4) * 8 + __builtin_ctz(m_4);
                        if (auto m_5 = m_x8_5.reduce()) return (ix8 + 5) * 8 + __builtin_ctz(m_5);
                        if (auto m_6 = m_x8_6.reduce()) return (ix8 + 6) * 8 + __builtin_ctz(m_6);
                        if (auto m_7 = m_x8_7.reduce()) return (ix8 + 7) * 8 + __builtin_ctz(m_7);

                        lu_assert(false);
                        return (usize) -1;
                    }
                }
            }

            return idx;
        }, [&](u32 result) { return result == min_idx_actual; });

        lu_benchmark("min idx simd x16", byte_size, [&] {
            auto min         = src_arr[0];
            auto min_block_i = (usize)0;
            auto max_u32_x8  = mm_u32x8_broadcast(-1);

            // find block with min value
            for (usize bi = 0; bi < count_blocks; bi += 1) {
                auto min_x8_0  = max_u32_x8;
                auto min_x8_1  = max_u32_x8;
                auto min_x8_2  = max_u32_x8;
                auto min_x8_3  = max_u32_x8;
                auto min_x8_4  = max_u32_x8;
                auto min_x8_5  = max_u32_x8;
                auto min_x8_6  = max_u32_x8;
                auto min_x8_7  = max_u32_x8;
                auto min_x8_8  = max_u32_x8;
                auto min_x8_9  = max_u32_x8;
                auto min_x8_10 = max_u32_x8;
                auto min_x8_11 = max_u32_x8;
                auto min_x8_12 = max_u32_x8;
                auto min_x8_13 = max_u32_x8;
                auto min_x8_14 = max_u32_x8;
                auto min_x8_15 = max_u32_x8;

                auto first =  bi      * block_size_u32x8;
                auto last  = (bi + 1) * block_size_u32x8;
                for (auto ix8 = first; ix8 < last; ix8 += 16) {
                    min_x8_0  = mm_min(min_x8_0 , src_x8[ix8 +  0]);
                    min_x8_1  = mm_min(min_x8_1 , src_x8[ix8 +  1]);
                    min_x8_2  = mm_min(min_x8_2 , src_x8[ix8 +  2]);
                    min_x8_3  = mm_min(min_x8_3 , src_x8[ix8 +  3]);
                    min_x8_4  = mm_min(min_x8_4 , src_x8[ix8 +  4]);
                    min_x8_5  = mm_min(min_x8_5 , src_x8[ix8 +  5]);
                    min_x8_6  = mm_min(min_x8_6 , src_x8[ix8 +  6]);
                    min_x8_7  = mm_min(min_x8_7 , src_x8[ix8 +  7]);
                    min_x8_8  = mm_min(min_x8_8 , src_x8[ix8 +  8]);
                    min_x8_9  = mm_min(min_x8_9 , src_x8[ix8 +  9]);
                    min_x8_10 = mm_min(min_x8_10, src_x8[ix8 + 10]);
                    min_x8_11 = mm_min(min_x8_11, src_x8[ix8 + 11]);
                    min_x8_12 = mm_min(min_x8_12, src_x8[ix8 + 12]);
                    min_x8_13 = mm_min(min_x8_13, src_x8[ix8 + 13]);
                    min_x8_14 = mm_min(min_x8_14, src_x8[ix8 + 14]);
                    min_x8_15 = mm_min(min_x8_15, src_x8[ix8 + 15]);
                }

                auto block_min = mm_min(
                    mm_min(
                        mm_min(mm_min(min_x8_0, min_x8_1), mm_min(min_x8_2, min_x8_3)),
                        mm_min(mm_min(min_x8_4, min_x8_5), mm_min(min_x8_6, min_x8_7))
                    ),
                    mm_min(
                        mm_min(mm_min(min_x8_8 , min_x8_9 ), mm_min(min_x8_10, min_x8_11)),
                        mm_min(mm_min(min_x8_12, min_x8_13), mm_min(min_x8_14, min_x8_15))
                    )
                ).hmin();

                if (block_min < min) {
                    min = block_min;
                    min_block_i = bi;
                }
            }

            // find idx within the block
            auto idx = (usize)-1;
            {
                auto target_x8 = mm_u32x8_broadcast(min);
                auto first =  min_block_i      * block_size_u32x8;
                auto last  = (min_block_i + 1) * block_size_u32x8;

                for (usize ix8 = first; ix8 < last; ix8 += 16) {
                    auto
                        m_x8_0  = target_x8 == src_x8[ix8 +  0],
                        m_x8_1  = target_x8 == src_x8[ix8 +  1],
                        m_x8_2  = target_x8 == src_x8[ix8 +  2],
                        m_x8_3  = target_x8 == src_x8[ix8 +  3],
                        m_x8_4  = target_x8 == src_x8[ix8 +  4],
                        m_x8_5  = target_x8 == src_x8[ix8 +  5],
                        m_x8_6  = target_x8 == src_x8[ix8 +  6],
                        m_x8_7  = target_x8 == src_x8[ix8 +  7],
                        m_x8_8  = target_x8 == src_x8[ix8 +  8],
                        m_x8_9  = target_x8 == src_x8[ix8 +  9],
                        m_x8_10 = target_x8 == src_x8[ix8 + 10],
                        m_x8_11 = target_x8 == src_x8[ix8 + 11],
                        m_x8_12 = target_x8 == src_x8[ix8 + 12],
                        m_x8_13 = target_x8 == src_x8[ix8 + 13],
                        m_x8_14 = target_x8 == src_x8[ix8 + 14],
                        m_x8_15 = target_x8 == src_x8[ix8 + 15]
                    ;

                    auto m_x8_all =
                          (((m_x8_0 | m_x8_1) | (m_x8_2  | m_x8_3 )) | ((m_x8_4  | m_x8_5 ) | (m_x8_6  | m_x8_7 )))
                        | (((m_x8_8 | m_x8_9) | (m_x8_10 | m_x8_11)) | ((m_x8_12 | m_x8_13) | (m_x8_14 | m_x8_15)));

                    if (m_x8_all.reduce()) {
                        if (auto m_0  = m_x8_0 .reduce()) return (ix8 +  0) * 8 + __builtin_ctz(m_0 );
                        if (auto m_1  = m_x8_1 .reduce()) return (ix8 +  1) * 8 + __builtin_ctz(m_1 );
                        if (auto m_2  = m_x8_2 .reduce()) return (ix8 +  2) * 8 + __builtin_ctz(m_2 );
                        if (auto m_3  = m_x8_3 .reduce()) return (ix8 +  3) * 8 + __builtin_ctz(m_3 );
                        if (auto m_4  = m_x8_4 .reduce()) return (ix8 +  4) * 8 + __builtin_ctz(m_4 );
                        if (auto m_5  = m_x8_5 .reduce()) return (ix8 +  5) * 8 + __builtin_ctz(m_5 );
                        if (auto m_6  = m_x8_6 .reduce()) return (ix8 +  6) * 8 + __builtin_ctz(m_6 );
                        if (auto m_7  = m_x8_7 .reduce()) return (ix8 +  7) * 8 + __builtin_ctz(m_7 );
                        if (auto m_8  = m_x8_8 .reduce()) return (ix8 +  8) * 8 + __builtin_ctz(m_8 );
                        if (auto m_9  = m_x8_9 .reduce()) return (ix8 +  9) * 8 + __builtin_ctz(m_9 );
                        if (auto m_10 = m_x8_10.reduce()) return (ix8 + 10) * 8 + __builtin_ctz(m_10);
                        if (auto m_11 = m_x8_11.reduce()) return (ix8 + 11) * 8 + __builtin_ctz(m_11);
                        if (auto m_12 = m_x8_12.reduce()) return (ix8 + 12) * 8 + __builtin_ctz(m_12);
                        if (auto m_13 = m_x8_13.reduce()) return (ix8 + 13) * 8 + __builtin_ctz(m_13);
                        if (auto m_14 = m_x8_14.reduce()) return (ix8 + 14) * 8 + __builtin_ctz(m_14);
                        if (auto m_15 = m_x8_15.reduce()) return (ix8 + 15) * 8 + __builtin_ctz(m_15);

                        lu_assert(false);
                        return (usize) -1;
                    }
                }
            }

            return idx;
        }, [&](u32 result) { return result == min_idx_actual; });
    }

    return 0;
}
