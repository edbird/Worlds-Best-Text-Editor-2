#ifndef DOCUMENT_LAYOUT_HPP
#define DOCUMENT_LAYOUT_HPP

#include "spdlog_util.hpp"

#include "document_layout_line.hpp"
#include "document_layout_cursor.hpp"

namespace TextLayoutEngine {

    struct DocumentLayout {

        DocumentLayout()
            : lines{std::vector<DocumentLayoutLine>()}
            , document_layout_cursor{DocumentLayoutCursor()}
        {
        }

        std::vector<DocumentLayoutLine> lines;
        DocumentLayoutCursor document_layout_cursor;
    };

}

#endif