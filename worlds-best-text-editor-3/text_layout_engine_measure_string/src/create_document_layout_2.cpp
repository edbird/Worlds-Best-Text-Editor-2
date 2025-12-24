#include "spdlog_util.hpp"

#include "create_document_layout_2.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <tuple>
#include <functional>

#include "document.hpp"
#include "document_layout.hpp"
#include "wrap_line.hpp"


bool calculate_caret_width(
    TTF_Font* const ttf_font,
    std::string text_under_caret,
    const int text_area_width_in_pixels,
    int &w
) {

    // This is a bit of a hack to handle the case where the cursor is 1 position past the end
    if (text_under_caret.size() < 1) {
        text_under_caret = "_";
    }

    SPDLOG_INFO("text_under_caret={}", text_under_caret);
    SPDLOG_INFO("size: {}", text_under_caret.size());
    if (
        !calculate_text_width_in_pixels_and_length(
            ttf_font,
            text_under_caret.c_str(),
            text_under_caret.size(),
            text_area_width_in_pixels,
            &w,
            nullptr
        )
    ) {
        SPDLOG_ERROR("failed to calculate text width");
        return false;
    }

    return true;
}

std::string get_text_under_cursor(
    const Document& document
) {
    const auto &cursor_line{document.lines[document.document_cursor.line_index]};
    if (document.document_cursor.column_index < cursor_line.size()) {
        const auto text_under_cursor{cursor_line.substr(document.document_cursor.column_index, 1)};
        return text_under_cursor;
    }
    else {
        const auto text_under_cursor{"_"};
        return text_under_cursor;
    }
}

// TODO: factor some of these parts into sub functions
TextLayoutEngine::DocumentLayout TextLayoutEngine::create_document_layout_2(
    TTF_Font* ttf_font,
    const int font_line_skip,
    const Document &document,
    const int text_area_width_in_pixels
) {
    auto &document_cursor{document.document_cursor};

    auto document_layout{DocumentLayout()};

    int y{0};
    const int dy{font_line_skip};

    for (const auto& [line_index, line]: std::ranges::enumerate_view(document.lines)) {

        SPDLOG_INFO("line_index={}", line_index);
        SPDLOG_INFO("line={}", line);

        const auto text_under_cursor{get_text_under_cursor(document)};

        int y_copy{y};

        std::vector<DocumentLayoutLine> wrapped_lines = wrap_line(
            ttf_font,
            font_line_skip,
            line,
            line_index,
            text_area_width_in_pixels,
            y,
            dy
        );

        for (const auto& [i, document_layout_line]: std::ranges::enumerate_view(wrapped_lines)) {

            SPDLOG_INFO("document_cursor={}", std::format("{}", document_cursor));

            if (document_cursor.line_index == document_layout_line.line_index) {

                SPDLOG_INFO("document_cursor.line_index == document_layout_line.line_index = {}", document_layout_line.line_index);

                const auto text_span{document_layout_line.text_span};
                const auto text_span_length = text_span.size();

                SPDLOG_INFO("text span: {}", text_span);
                SPDLOG_INFO("text span length: {}", text_span_length);

                if (document_cursor.column_index >= document_layout_line.column_index &&
                    document_cursor.column_index <= document_layout_line.column_index + text_span_length)
                    // Comparison is <= because cursor must be able to move 1 position beyond end of line
                {
                    SPDLOG_INFO("document_cursor.column_index = {}", document_cursor.column_index);

                    const auto column_index{
                        document_cursor.column_index - document_layout_line.column_index
                    };
                    SPDLOG_INFO("column_index={}", column_index);

                    int w{0};
                    /*std::string text_under_caret = std::string(
                        text_span.substr(column_index, 1)
                    );*/
                    if (
                        !calculate_caret_width(
                            ttf_font,
                            //std::move(text_under_caret),
                            std::move(text_under_cursor),
                            text_area_width_in_pixels,
                            w
                        )
                    ) {
                        SPDLOG_ERROR("failed to calculate caret width");
                    }
                    SPDLOG_INFO("w={}", w);

                    int x{0};

                    std::string tmp_text = std::string(
                        text_span.substr(0, column_index)
                    );
                    SPDLOG_INFO("tmp_text={}", tmp_text);

                    if (
                        !calculate_text_width_in_pixels_and_length(
                            ttf_font,
                            tmp_text.c_str(),
                            tmp_text.size(),
                            text_area_width_in_pixels,
                            &x,
                            nullptr
                        )
                    ) {
                        SPDLOG_ERROR("failed to calculate text width");
                    }
                    SPDLOG_INFO("x={}", x);

                    // TODO: <= or < ?
                    if (x + w <= text_area_width_in_pixels) {
                        // ok
                    }
                    else {
                        // cursor off end of screen
                        SPDLOG_INFO("cursor is off end of screen");
                        x = 0;
                        y_copy += dy;
                    }

                    TextLayoutEngine::DocumentLayoutCursor document_layout_cursor(
                        DocumentLayoutCursorType::BLOCK,
                        static_cast<std::size_t>(i),
                        column_index,
                        x,
                        y_copy,
                        w,
                        font_line_skip
                    );

                    SPDLOG_INFO("document_layout_cursor: x={}, y={}", x, y_copy);

                    document_layout.document_layout_cursor = std::move(document_layout_cursor);
                }
                else {
                    y_copy += dy;
                }
            }
            else {
                y_copy += dy;
            }

            //y += dy;
            //y_copy += dy;
        }

        document_layout.lines.append_range(std::move(wrapped_lines));
    }

    return document_layout;
}