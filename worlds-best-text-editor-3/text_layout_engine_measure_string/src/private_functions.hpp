#ifndef TEXT_LAYOUT_ENGINE_MEASURE_STRING_PRIVATE_FUNCTIONS_HPP
#define TEXT_LAYOUT_ENGINE_MEASURE_STRING_PRIVATE_FUNCTIONS_HPP

#include "spdlog_util.hpp"

#include <SDL3_ttf/SDL_ttf.h>

bool calculate_text_width_in_pixels_and_length(
    TTF_Font* ttf_font,
    const char* text,
    const std::size_t text_length,
    const int max_width_pixels,
    int *const measured_width_pixels,
    std::size_t *const measured_length
);

#endif