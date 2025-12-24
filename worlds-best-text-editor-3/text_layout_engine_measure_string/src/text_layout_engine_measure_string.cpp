#include "spdlog_util.hpp"

#include "text_layout_engine_measure_string.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <tuple>
#include <functional>

#include "wrap_line.hpp"


TextLayoutEngine::DocumentLayout TextLayoutEngine::create_document_layout(
    TTF_Font* ttf_font,
    const int font_line_skip,
    const Document &document,
    const int text_area_width_in_pixels
) {
    auto document_layout{DocumentLayout()};

    int y{0};
    const int dy{font_line_skip};

    std::size_t line_index = 0;
    for (const auto& line: document.lines) {
        SPDLOG_DEBUG("line_index={}", line_index);

        std::vector<DocumentLayoutLine> wrapped_lines = wrap_line(
            ttf_font,
            font_line_skip,
            line,
            line_index,
            text_area_width_in_pixels,
            y,
            dy
        );

        document_layout.lines.append_range(std::move(wrapped_lines));

        ++ line_index;
    }

    return document_layout;
}


// TODO: factor some of these parts into sub functions
TextLayoutEngine::DocumentLayoutCursor TextLayoutEngine::convert_document_cursor_to_document_layout_cursor(
    TTF_Font* ttf_font,
    const int font_line_skip,
    const DocumentLayout& document_layout,
    const DocumentCursor& document_cursor,
    const int text_area_width_in_pixels
) {
    // TODO: this is temporary, remove
    SPDLOG_ERROR("should never reach this line");
    return TextLayoutEngine::DocumentLayoutCursor();

    SPDLOG_INFO("start of cursor position algorithm");
    SPDLOG_INFO("document_cursor={}", std::format("{}", document_cursor));

    int y{0};
    const int dy{font_line_skip};

    for (const auto& [i, document_layout_line]: std::ranges::enumerate_view(document_layout.lines)) {
        if (document_cursor.line_index == document_layout_line.line_index) {
            const auto text_span{document_layout_line.text_span};
            const auto text_span_length = text_span.size();

            if (document_cursor.column_index >= document_layout_line.column_index &&
                document_cursor.column_index <= document_layout_line.column_index + text_span_length)
                // Comparison is <= because cursor must be able to move 1 position beyond end of line
            {

                const auto column_index{
                    document_cursor.column_index - document_layout_line.column_index
                };

                int w{0};
                std::string text_under_caret = std::string(
                    text_span.substr(column_index, 1)
                );

                // This is a bit of a hack to handle the case where the cursor is 1 position past the end
                if (text_under_caret.size() < 1) {
                    text_under_caret = "_";
                }

                SPDLOG_INFO("text_under_caret={}", text_under_caret);
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
                    SPDLOG_ERROR("failed to calculate number of characters that fit on line"); // TODO wrong message
                }

                int x{0};

                if (column_index > 0) {
                    // Exceptional case for column 0 required because passing 0
                    // for string length to TTF_MeasureString implies string
                    // length determined by position of null in null terminated
                    // string

                    std::string tmp_text_2 = std::string(
                        text_span.substr(0, column_index)
                    );

                    if (
                        !calculate_text_width_in_pixels_and_length(
                            ttf_font,
                            tmp_text_2.c_str(),
                            tmp_text_2.size(),
                            text_area_width_in_pixels,
                            &x,
                            nullptr
                        )
                    ) {
                        SPDLOG_ERROR("failed to calculate number of characters that fit on line"); // TODO wrong message
                    }
                }
                else {
                    // column 0
                    // do nothing
                }

                // TODO: <= or < ?
                if (x + w <= text_area_width_in_pixels) {
                    // ok
                }
                else {
                    // cursor off end of screen
                    x = 0;
                    y += dy;
                }

                TextLayoutEngine::DocumentLayoutCursor document_layout_cursor(
                    DocumentLayoutCursorType::BLOCK,
                    static_cast<std::size_t>(i),
                    column_index,
                    x,
                    y,
                    w,
                    font_line_skip
                );

                return document_layout_cursor;
            }
        }

        y += dy;
    }

    return TextLayoutEngine::DocumentLayoutCursor();
}


        /*auto [cursor_line_index, cursor_column_index]{document.document_cursor};

        std::size_t accumulated_chars{0};
        std::size_t layout_row{0};

        for (const auto& layout_line: document_layout.lines) {
            if (layout_line.line_index == cursor_line_index) {
                const std::size_t span_len = layout_line.text_span.size();

                if (cursor_column_index >= accumulated_chars + span_len) {
                    // cursor is after this wrapped span; accumulate and continue
                    accumulated_chars += span_len;
                }
                else {
                    // cursor lies within this wrapped span
                    document_cursor.line_index = layout_row;
                    document_cursor.column_index = cursor_column_index - accumulated_chars;
                    break;
                }
            }

            ++ document_cursor.line_index;
        }*/


bool TextLayoutEngine::draw_document_layout(
    SDL_Renderer* renderer, // can be removed
    TTF_Text* ttf_text,
    const int font_line_skip,
    const int text_area_width_in_pixels,
    const int text_area_height_in_pixels,
    const DocumentLayout& document_layout,
    const std::size_t start_line
) {

    for (const auto& [line_index, line]: std::ranges::enumerate_view(document_layout.lines)) {

        // TODO: scrolling should be done using pixels
        if (line_index < static_cast<int64_t>(start_line)) {
            continue;
        }

        const auto x{line.x};
        const auto y{line.y};
        const auto line_width_in_pixels = line.w;
        const auto line_height_in_pixels = line.h;

        if (0 + line_width_in_pixels <= text_area_width_in_pixels) {

        }
        else {
            SPDLOG_WARN("skipping rendering of line {}, width is {} which exceeds maximum width of {}",
                line_index,
                0 + line_width_in_pixels,
                text_area_width_in_pixels
            );
            continue;
        }
        if (y + line_height_in_pixels <= text_area_height_in_pixels) {

        }
        else {
            break;
        }

        const auto line_text = line.text_span;
        //SPDLOG_INFO("line_text={}", line_text);
        if (!TTF_SetTextString(ttf_text, line_text.data(), line_text.size())) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("failed to set text string: {}", error);
            return false;
        }
                                        /*  VVVV -- TODO: change to line.x */
        if (!TTF_DrawRendererText(ttf_text, 0.0f, static_cast<float>(y))) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("failed to draw text: {}", error);
            return false;
        }
    }

    return true;
}

bool TextLayoutEngine::draw_document_cursor(
    SDL_Renderer* renderer,
    const DocumentLayoutCursor& document_layout_cursor
) {

    const auto x{document_layout_cursor.x};
    const auto y{document_layout_cursor.y};
    const auto w{document_layout_cursor.w};
    const auto h{document_layout_cursor.h};

    const SDL_FRect rect{
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(w),
        static_cast<float>(h)
    };
    SDL_RenderRect(renderer, &rect);

    //SPDLOG_INFO("render rect: {} {} {} {}", x, y, w, h);

    // TODO: error handling

    return true;
}

        /*if (line_index == cursor_line_index) {
            // TODO: check cursor_x < actual length?
            int measured_width{0};
            const auto ttf_font = TTF_GetTextFont(ttf_text);
            if (!TTF_MeasureString(ttf_font, line_text.data(), cursor_column_index, text_area_width_in_pixels, &measured_width, nullptr)) {
                const auto error = SDL_GetError();
                SPDLOG_ERROR("TTF_MeasureString error: {}", error);
                return false;
            }
            // Required because length = 0 means null-terminated string
            if (cursor_column_index == 0) {
                measured_width = 0;
            }
            //SPDLOG_INFO("cursor x placement: measured_width={}", measured_width);

            const auto y{font_line_skip * cursor_line_index};
            const auto x{measured_width};

            // TODO: cache this
            constexpr std::string_view W_string("W");
            int w{0};
            if (!TTF_MeasureString(ttf_font, W_string.data(), W_string.size(), 100, &w, nullptr)) {
                const auto error = SDL_GetError();
                SPDLOG_ERROR("TTF_MeasureString error: {}", error);
                return false;
            }
            const int h{font_line_skip};
            const SDL_FRect rect{static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)};
            SDL_RenderRect(renderer, &rect);
        }*/