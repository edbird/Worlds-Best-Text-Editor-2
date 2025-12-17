#ifndef TEXT_LAYOUT_ENGINE_HPP
#define TEXT_LAYOUT_ENGINE_HPP

#include "spdlog_util.hpp"

#include "document.hpp"

#include <SDL3_ttf/SDL_ttf.h>

namespace TextLayoutEngine {

    struct DocumentLayoutLine {
        std::size_t line_index;
        std::string_view text_span;
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

    DocumentLayout create_document_layout(
        const Document& document,
        TTF_Text* ttf_text,
        const int width_in_pixels
    );

}

#endif