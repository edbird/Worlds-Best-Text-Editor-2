#ifndef PERFORMANCE_UTIL_HPP
#define PERFORMANCE_UTIL_HPP

#include "spdlog_util.hpp"

#include <SDL3/SDL.h>

#include <cstdint>

struct FrameCounter {

    FrameCounter()
        : frame_count{0}
    {
    }

    uint64_t frame_count;
};

void reset_frame_counter(FrameCounter& frame_counter);

void increment_frame_counter(FrameCounter& frame_counter);

struct PerformanceTimer {

    PerformanceTimer()
        : performance_counter{0}
        , performance_counter_frequency{0}
        , performance_counter_frequency_float{static_cast<double>(0)}
    {
    }

    PerformanceTimer(const Uint64 performance_counter, const Uint64 performance_counter_frequency)
        : performance_counter{performance_counter}
        , performance_counter_frequency{performance_counter_frequency}
        , performance_counter_frequency_float{static_cast<double>(performance_counter_frequency)}
    {
    }

    Uint64 performance_counter;
    Uint64 performance_counter_frequency;
    double performance_counter_frequency_float;
};

double get_frame_rate(const FrameCounter& frame_counter, const PerformanceTimer& performance_timer, const Uint64 performance_counter);

double get_time_elapsed_in_seconds(const PerformanceTimer& performance_timer, const Uint64 performance_counter);

void update_performance_timer(PerformanceTimer& performance_timer, const Uint64 performance_counter);


#endif