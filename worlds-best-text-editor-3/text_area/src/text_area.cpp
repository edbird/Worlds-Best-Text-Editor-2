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
}

void TextArea::handle_command(Command& command) {
    SPDLOG_INFO("handle_command: {}, {}", std::format("{}", command.command_type), command.utf8_character);

    if (command.command_type == CommandType::INSERT_CHAR) {
        if (optional_document.has_value()) {
            auto document{optional_document.value()};
            document.get().insert_char(command.utf8_character);
        }

        update_document_layout();
    }
    else if (command.command_type == CommandType::DELETE) {
        if (optional_document.has_value()) {
            auto document{optional_document.value()};
            document.get().delete_();
        }

        update_document_layout();
    }
    else if (command.command_type == CommandType::UP) {
        if (optional_document.has_value()) {
            auto document{optional_document.value()};
            document.get().cursor_up();
        }

        update_document_layout();
    }
    else if (command.command_type == CommandType::DOWN) {
        if (optional_document.has_value()) {
            auto document{optional_document.value()};
            document.get().cursor_down();
        }

        update_document_layout();
    }
    else if (command.command_type == CommandType::LEFT) {
        if (optional_document.has_value()) {
            auto document{optional_document.value()};
            document.get().cursor_left();
        }

        update_document_layout();
    }
    else if (command.command_type == CommandType::RIGHT) {
        if (optional_document.has_value()) {
            auto document{optional_document.value()};
            document.get().cursor_right();
        }

        update_document_layout();
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
        optional_document,
        width_in_pixels
    );

    //log_rendering_result();
}

void TextArea::log_rendering_result() {
    SPDLOG_INFO("rendering result:");
    for (const auto& line: document_layout.lines) {
        SPDLOG_INFO("{}", line.text_span);
    }
}