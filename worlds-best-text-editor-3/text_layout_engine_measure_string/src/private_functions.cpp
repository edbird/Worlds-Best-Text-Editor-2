#include "spdlog_util.hpp"

#include "private_functions.hpp"

#include <SDL3_ttf/SDL_ttf.h>


bool calculate_text_width_in_pixels_and_length(
    TTF_Font* ttf_font,
    const char* text,
    const std::size_t text_length,
    const int max_width_pixels,
    int *const measured_width_pixels,
    std::size_t *const measured_length
) {
    SPDLOG_WARN("text_length={}", text_length);

    // if (!TTF_SetTextString(ttf_text, text.data(), text.size())) {
    //     throw std::runtime_error("TTF_SetTextString failure");
    // }
    // int w = 0;
    // int h = 0;
    // if (!TTF_GetTextSize(ttf_text, &w, &h)) {
    //     throw std::runtime_error("TTF_GetTextSize failure");
    // }
    // return std::make_tuple(w, h);

    // TTF_MeasureString interprets 0 length string parameter as meaning string
    // is null terminated and length should be calculated first.
    if (text_length == 0) {
        SPDLOG_INFO("text_length 0, return measured length 0");
        if (measured_width_pixels) {
            *measured_width_pixels = 0;
        }
        if (measured_length) {
            *measured_length = 0;
        }
        SPDLOG_INFO("return true");
        return true;

        // This comment moved from TextLayoutEngine
        //
        // Exceptional case for column 0 required because passing 0
        // for string length to TTF_MeasureString implies string
        // length determined by position of null in null terminated
        // string
    }

    if (!TTF_MeasureString(
            ttf_font,
            text,
            text_length,
            max_width_pixels,
            measured_width_pixels,
            measured_length
        )
    ) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("measure string failed: {}", error);
        SPDLOG_INFO("return false");
        return false;
    }

    SPDLOG_INFO("return true");
    return true;
}