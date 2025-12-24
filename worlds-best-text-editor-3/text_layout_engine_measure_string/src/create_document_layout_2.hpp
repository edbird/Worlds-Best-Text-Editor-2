#ifndef CREATE_DOCUMENT_LAYOUT_2_HPP
#define CREATE_DOCUMENT_LAYOUT_2_HPP

#include "spdlog_util.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include "document.hpp"
#include "document_layout.hpp"

namespace TextLayoutEngine{

    DocumentLayout create_document_layout_2(
        TTF_Font* ttf_font,
        const int font_line_skip,
        const Document &document,
        const int text_area_width_in_pixels
    );

}

#endif