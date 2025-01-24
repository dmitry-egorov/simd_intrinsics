// Lanski's Utilities
//     by Dmitry Egorov
#pragma once

#include "lu_prims.h"
#include "lu_timer.h"

static inline auto lu_benchmark(const char* name, usize bytes, auto fn, auto validate) -> void {
    lu_init_cpu_timer();

    printf("%-20s: ", name);

    auto overall_duration = lu_cpu_timer_freq; // 1 second

    auto overall_start_time = lu_cpu_read_timer();
    auto min_time_s = 100000000000000.0;
    auto total_duration = (u64)0;
    auto iterations_count = (usize)0;
    for (; lu_cpu_read_timer() - overall_start_time < overall_duration; iterations_count++) {
        auto start_time = lu_cpu_read_timer();
        auto result     = fn();
        auto end_time   = lu_cpu_read_timer();
        auto duration   = (end_time - start_time);

        total_duration += duration;

        auto time_s = (f64)duration / (f64)lu_cpu_timer_freq;

        if (time_s < min_time_s) {
            min_time_s = time_s;
        }

        if (!validate(result)) {
            printf("Unexpected result! %d\n", (u32)result);
            return;
        }
    }

    auto avg_time_s = (f64)total_duration / (f64)(lu_cpu_timer_freq * iterations_count);

    auto min_time_us    = (u32)(min_time_s * 1'000'000.0);
    auto avg_time_us    = (u32)(avg_time_s * 1'000'000.0);
    auto min_throughput = ((f64)bytes / (f64)min_time_s) / 1'000'000'000.0;
    auto avg_throughput = ((f64)bytes / (f64)avg_time_s) / 1'000'000'000.0;


    printf("min %8dus, %10f Gb/s, avg %8dus, %10f Gb/s\n", min_time_us, min_throughput, avg_time_us, avg_throughput);
}