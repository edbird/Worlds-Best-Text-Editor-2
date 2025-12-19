#include "spdlog_util.hpp"

#include "text_area.hpp"

#include "text_layout_engine_measure_string.hpp"
#include "window_geometry.hpp"
#include "document.hpp"

TextArea::~TextArea() {

}

void TextArea::draw() const {
    //SPDLOG_INFO("font_line_skip={}, w={}, h={}, start_line={}, ttf_text={:p}",
    //    font_line_skip, width_in_pixels, height_in_pixels, start_line, static_cast<void*>(ttf_text));

    draw_document_layout(
        ttf_text,
        font_line_skip,
        width_in_pixels,
        height_in_pixels,
        document_layout,
        start_line
    );
}


void TextArea::frame_update() {
    ++ frame_count;
}

void TextArea::update_document(
    const Document& document,
    const WindowGeometry& window_geometry
) {

    width_in_pixels = window_geometry.screen_width_in_pixels;
    height_in_pixels = window_geometry.screen_height_in_pixels;

    const auto ttf_font{TTF_GetTextFont(ttf_text)};

    document_layout = create_document_layout(
        ttf_font,
        document,
        width_in_pixels
    );

    log_rendering_result();
}

void TextArea::log_rendering_result() {
    SPDLOG_INFO("rendering result:");
    for (const auto& line: document_layout.lines) {
        SPDLOG_INFO("{}", line.text_span);
    }
}