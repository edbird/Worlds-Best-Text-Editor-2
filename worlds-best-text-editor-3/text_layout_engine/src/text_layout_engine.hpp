#ifndef TEXT_LAYOUT_ENGINE_HPP
#define TEXT_LAYOUT_ENGINE_HPP

#include "spdlog_util.hpp"

#include "document.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <string_view>

namespace TextLayoutEngine {

    struct DocumentLayoutLine {
        const std::size_t line_index;
        const std::size_t column_index;
        const std::string_view text_span;
        const int width_pixels;
        const int height_pixels;
    };

    struct DocumentLayout {

        std::vector<DocumentLayoutLine> lines;
    };

    DocumentLayout create_document_layout(
        TTF_Text* ttf_text,
        const Document& document,
        const std::size_t width_in_pixels
    );

}

#endif