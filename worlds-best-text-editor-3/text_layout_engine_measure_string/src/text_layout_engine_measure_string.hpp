#ifndef TEXT_LAYOUT_ENGINE_MEASURE_STRING_HPP
#define TEXT_LAYOUT_ENGINE_MEASURE_STRING_HPP

#include "spdlog_util.hpp"

#include "document.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <string_view>
#include <functional>
#include <optional>
#include <format>

namespace TextLayoutEngine {

    // TODO:
    //
    // This structure contains information about where lines are positioned
    // in the (line_index, column_index) space as well as the (pixel_x, pixel_y)
    // space
    struct DocumentLayoutLine {
        std::size_t line_index;
        std::size_t column_index;
        std::string_view text_span;
        int x;
        int y;
        int w;
        int h;
    };

    struct DocumentLayoutCursorPosition {

        DocumentLayoutCursorPosition()
            : line_index{0ul}
            , column_index{0ul}
            , x{0}
            , y{0}
            , w{0}
            , h{0}
        {
        }

        DocumentLayoutCursorPosition(
            std::size_t line_index,
            std::size_t column_index,
            int x,
            int y,
            int w,
            int h
        )
            : line_index{line_index}
            , column_index{column_index}
            , x{x}
            , y{y}
            , w{w}
            , h{h}
        {
        }

        std::size_t line_index;
        std::size_t column_index;
        int x;
        int y;
        int w;
        int h;
    };

    struct DocumentLayout {

        DocumentLayout()
            : lines{std::vector<DocumentLayoutLine>()}
            , document_layout_cursor_position{DocumentLayoutCursorPosition()}
        {
        }

        std::vector<DocumentLayoutLine> lines;
        DocumentLayoutCursorPosition document_layout_cursor_position;
    };

    DocumentLayout create_document_layout(
        TTF_Font* ttf_font,
        const int font_line_skip,
        const Document &document,
        const int text_area_width_in_pixels
    );

    DocumentLayoutCursorPosition convert_document_cursor_position_to_document_layout_cursor_position(
        TTF_Font* ttf_font,
        const int font_line_skip,
        const DocumentLayout& document_layout,
        const DocumentCursorPosition& document_cursor_position,
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
        const DocumentLayoutCursorPosition& document_layout_cursor_positions
    );

}

template<>
struct std::formatter<TextLayoutEngine::DocumentLayoutCursorPosition> : std::formatter<std::string> {
    auto format(const TextLayoutEngine::DocumentLayoutCursorPosition& document_layout_cursor_position, auto& context) const {
        return std::format_to(
            context.out(),
            "(line={}, column={})",
            document_layout_cursor_position.line_index,
            document_layout_cursor_position.column_index
        );
    }
};

#endif