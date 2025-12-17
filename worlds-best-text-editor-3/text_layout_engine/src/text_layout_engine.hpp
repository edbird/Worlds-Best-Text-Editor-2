#ifndef TEXT_LAYOUT_ENGINE_HPP
#define TEXT_LAYOUT_ENGINE_HPP

#include "spdlog_util.hpp"

#include "document.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <string_view>

namespace TextLayoutEngine {

    // TODO: make members const?
    struct DocumentLayoutLine {
        std::size_t line_index;
        std::size_t char_index;
        std::string_view text_span;

        // TODO: wanted?
        //int width_pixels;
        //int height_pixels;
    };

    struct DocumentLayout {

        std::vector<DocumentLayoutLine> lines;
    };

    std::size_t calculate_wrap_length(
        TTF_Text* ttf_text,
        const int width_in_pixels,
        const std::string& line,
        const std::size_t start
    );

    std::vector<DocumentLayoutLine> wrap_line(
        TTF_Text* ttf_text,
        const std::string& line,
        const std::size_t line_index,
        std::size_t width_in_pixels
    );

    DocumentLayout create_document_layout(
        TTF_Text* ttf_text,
        const Document& document,
        const std::size_t width_in_pixels
    );

}

#endif