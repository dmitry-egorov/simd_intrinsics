// Lanski's Utilities
//     by Dmitry Egorov
#pragma once

#include <chrono>

static u64 lu_cpu_timer_freq = 0;

static inline u64 lu_cpu_read_timer() {
    return __rdtsc();
}

static inline u64 lu_estimate_cpu_timer_freq(usize wait_time_ms) {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    auto wait_time = milliseconds(wait_time_ms);

    auto cpu_start = lu_cpu_read_timer();
    auto start = high_resolution_clock::now();

    while(true) {
        auto elapsed = high_resolution_clock::now() - start;
        if (elapsed >= wait_time)
            return (lu_cpu_read_timer() - cpu_start) * (1s / elapsed);
    }

    return 0;
}

static inline void lu_init_cpu_timer(usize wait_time_ms = 300) {
    if (lu_cpu_timer_freq == 0); else return;

    lu_cpu_timer_freq = lu_estimate_cpu_timer_freq(wait_time_ms);
}