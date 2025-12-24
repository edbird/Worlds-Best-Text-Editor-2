#ifndef DOCUMENT_LAYOUT_CURSOR_HPP
#define DOCUMENT_LAYOUT_CURSOR_HPP

#include "spdlog_util.hpp"

namespace TextLayoutEngine {

    enum class DocumentLayoutCursorType {
        BLOCK,
        BAR
    };

    struct DocumentLayoutCursor {

        DocumentLayoutCursor()
            : cursor_type{DocumentLayoutCursorType::BLOCK}
            , line_index{0ul}
            , column_index{0ul}
            , x{0}
            , y{0}
            , w{0}
            , h{0}
        {
        }

        DocumentLayoutCursor(
            const DocumentLayoutCursorType cursor_type,
            std::size_t line_index,
            std::size_t column_index,
            int x,
            int y,
            int w,
            int h
        )
            : cursor_type{cursor_type}
            , line_index{line_index}
            , column_index{column_index}
            , x{x}
            , y{y}
            , w{w}
            , h{h}
        {
        }

        DocumentLayoutCursorType cursor_type;
        std::size_t line_index;
        std::size_t column_index;
        int x;
        int y;
        int w;
        int h;
    };

}

#endif