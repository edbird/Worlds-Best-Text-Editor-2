#ifndef WRAP_LINE_HPP
#define WRAP_LINE_HPP

#include "spdlog_util.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "document_layout_line.hpp"

// TODO: remove from public
bool calculate_text_width_in_pixels_and_length(
    TTF_Font* ttf_font,
    const char* text,
    const std::size_t text_length,
    const int max_width_pixels,
    int *const measured_width_pixels,
    std::size_t *const measured_length
);

std::vector<TextLayoutEngine::DocumentLayoutLine> wrap_line(
    TTF_Font* ttf_font,
    const int font_line_skip,
    const std::string& line,
    const std::size_t line_index,
    const int text_area_width_in_pixels,
    int &y,
    const int dy /* same as font_line_skip, but in principle may be different */
);

#endif