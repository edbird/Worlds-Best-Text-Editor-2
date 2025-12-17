#include "spdlog_util.hpp"

#include "text_area.hpp"

#include "window_geometry.hpp"
#include "document.hpp"

TextArea::~TextArea() {

}

void TextArea::draw() const {
    //SPDLOG_INFO("font_line_skip={}, w={}, h={}, start_line={}, ttf_text={:p}",
    //    font_line_skip, screen_width_in_pixels, screen_height_in_pixels, start_line, static_cast<void*>(ttf_text));

    draw_document_layout(
        ttf_text,
        font_line_skip,
        screen_width_in_pixels,
        screen_height_in_pixels,
        document_layout,
        start_line
    );
}

void TextArea::update_document(
    const Document& document,
    const WindowGeometry& window_geometry
) {

    const auto ttf_font{TTF_GetTextFont(ttf_text)};

    const auto document_layout = create_document_layout(
        ttf_font,
        document,
        window_geometry.screen_width_in_pixels
    );

    SPDLOG_INFO("rendering result:");
    for (const auto& line: document_layout.lines) {
        SPDLOG_INFO("{}", line.text_span);
    }

    this->document_layout = std::move(document_layout);
}