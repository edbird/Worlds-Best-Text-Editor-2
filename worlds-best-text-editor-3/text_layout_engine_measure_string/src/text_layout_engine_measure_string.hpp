#ifndef TEXT_LAYOUT_ENGINE_MEASURE_STRING_HPP
#define TEXT_LAYOUT_ENGINE_MEASURE_STRING_HPP

#include "spdlog_util.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <string_view>
#include <functional>
#include <optional>
#include <format>

#include "document.hpp"
#include "document_layout.hpp"
#include "document_layout_cursor.hpp"

namespace TextLayoutEngine {

    DocumentLayout create_document_layout_2(
        TTF_Font* ttf_font,
        const int font_line_skip,
        const Document &document,
        const int text_area_width_in_pixels
    );

    DocumentLayout create_document_layout(
        TTF_Font* ttf_font,
        const int font_line_skip,
        const Document &document,
        const int text_area_width_in_pixels
    );

    DocumentLayoutCursor convert_document_cursor_to_document_layout_cursor(
        TTF_Font* ttf_font,
        const int font_line_skip,
        const DocumentLayout& document_layout,
        const DocumentCursor& document_cursor,
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

    bool draw_document_cursor(
        SDL_Renderer* renderer,
        const DocumentLayoutCursor& document_layout_cursor
    );

}

template<>
struct std::formatter<TextLayoutEngine::DocumentLayoutCursor> : std::formatter<std::string> {
    auto format(const TextLayoutEngine::DocumentLayoutCursor& document_layout_cursor, auto& context) const {
        return std::format_to(
            context.out(),
            "(line={}, column={})",
            document_layout_cursor.line_index,
            document_layout_cursor.column_index
        );
    }
};

#endif