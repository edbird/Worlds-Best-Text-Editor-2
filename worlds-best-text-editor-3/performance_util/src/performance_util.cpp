#include "spdlog_util.hpp"

#include "performance_util.hpp"

void reset_frame_counter(FrameCounter& frame_counter) {
    frame_counter.frame_count = 0;
}

void increment_frame_counter(FrameCounter& frame_counter) {
    ++ frame_counter.frame_count;
}

double get_frame_rate(const FrameCounter& frame_counter, const PerformanceTimer& performance_timer, const Uint64 performance_counter) {
    const auto frame_count{frame_counter.frame_count};
    const auto time_in_seconds{get_time_elapsed_in_seconds(performance_timer, performance_counter)};
    const auto frame_rate = static_cast<double>(frame_count) / time_in_seconds;
    return frame_rate;
}

double get_time_elapsed_in_seconds(const PerformanceTimer& performance_timer, const Uint64 performance_counter) {

    const auto performance_counter_last{performance_timer.performance_counter};
    const auto performance_counter_frequency_float{performance_timer.performance_counter_frequency_float};
    const auto time_in_seconds = static_cast<double>(performance_counter - performance_counter_last) / performance_counter_frequency_float;
    return time_in_seconds;
}

void update_performance_timer(PerformanceTimer& performance_timer, const Uint64 performance_counter) {
    performance_timer.performance_counter = performance_counter;
}