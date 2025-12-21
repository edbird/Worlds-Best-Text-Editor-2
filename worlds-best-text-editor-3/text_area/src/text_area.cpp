#include "spdlog_util.hpp"

#include "text_area.hpp"

#include "text_layout_engine_measure_string.hpp"
#include "window_geometry.hpp"
#include "document.hpp"
#include "command.hpp"

TextArea::~TextArea() {

}

void TextArea::draw() const {
    //SPDLOG_INFO("font_line_skip={}, w={}, h={}, start_line={}, ttf_text={:p}",
    //    font_line_skip, width_in_pixels, height_in_pixels, start_line, static_cast<void*>(ttf_text));

    draw_document_layout(
        renderer,
        ttf_text,
        font_line_skip,
        width_in_pixels,
        height_in_pixels,
        document_layout,
        start_line
    );

    draw_document_cursor(
        renderer,
        document_layout.document_layout_cursor_position
    );
}

void TextArea::handle_command(Command& command) {
    SPDLOG_INFO("handle_command: {}, {}", std::format("{}", command.command_type), command.utf8_character);

    if (command.command_type == CommandType::INSERT_CHAR) {
        document.insert_char(command.utf8_character);

        update_document_layout();
        update_document_cursor_position();
    }
    else if (command.command_type == CommandType::DELETE) {
        document.delete_();

        update_document_layout();
        update_document_cursor_position();
    }
    else if (command.command_type == CommandType::UP) {
        document.cursor_up();

        update_document_layout();
        update_document_cursor_position();
    }
    else if (command.command_type == CommandType::DOWN) {
        document.cursor_down();

        update_document_layout();
        update_document_cursor_position();
    }
    else if (command.command_type == CommandType::LEFT) {
        document.cursor_left();

        update_document_layout();
        update_document_cursor_position();
    }
    else if (command.command_type == CommandType::RIGHT) {
        document.cursor_right();

        update_document_layout();
        update_document_cursor_position();
    }
    else {
        // TextArea does not handle this command
    }
}

void TextArea::frame_update() {
    ++ frame_count;
}

void TextArea::update_document_layout() {

    const auto ttf_font{TTF_GetTextFont(ttf_text)};

    document_layout = create_document_layout(
        ttf_font,
        font_line_skip,
        document,
        width_in_pixels
    );

    //log_rendering_result();
}

void TextArea::update_document_cursor_position() {

    // Note: this is by value
    const auto document_cursor_position{
        document.document_cursor
    };

    document_layout.document_layout_cursor_position =
        convert_document_cursor_position_to_document_layout_cursor_position(
            ttf_font,
            font_line_skip,
            document_layout,
            document_cursor_position,
            width_in_pixels
        );

}

void TextArea::log_rendering_result() {
    SPDLOG_INFO("rendering result:");
    for (const auto& line: document_layout.lines) {
        SPDLOG_INFO("{}", line.text_span);
    }
}