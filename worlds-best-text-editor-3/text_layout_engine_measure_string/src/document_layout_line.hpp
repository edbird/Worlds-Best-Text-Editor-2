#ifndef DOCUMENT_LAYOUT_LINE_HPP
#define DOCUMENT_LAYOUT_LINE_HPP

#include "spdlog_util.hpp"

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

}

#endif