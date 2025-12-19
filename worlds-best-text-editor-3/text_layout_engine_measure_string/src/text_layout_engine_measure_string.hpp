#ifndef TEXT_LAYOUT_ENGINE_MEASURE_STRING_HPP
#define TEXT_LAYOUT_ENGINE_MEASURE_STRING_HPP

#include "spdlog_util.hpp"

#include "document.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <string_view>
#include <functional>
#include <optional>

namespace TextLayoutEngine {

    struct DocumentLayoutLine {
        std::size_t line_index;
        std::size_t char_index;
        std::string_view text_span;
        int width_pixels;
        int height_pixels;
    };

    struct DocumentLayout {

        std::vector<DocumentLayoutLine> lines;
        std::tuple<std::size_t, std::size_t> cursor;
    };

    DocumentLayout create_document_layout(
        TTF_Font* ttf_font,
        const std::optional<std::reference_wrapper<Document>> optional_document,
        const int text_area_width_in_pixels
    );

    bool draw_document_layout(
        SDL_Renderer* renderer,
        TTF_Text* ttf_text,
        const int font_line_skip,
        const int text_area_width_in_pixels,
        const int text_area_height_in_pixels,
        const DocumentLayout& document_layout,
        const std::size_t start_line
    );

}

#endif