#include "spdlog_util.hpp"

#include "text_layout_engine_measure_string.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <tuple>
#include <functional>
#include <optional>

bool calculate_text_width_in_pixels_and_length(
    TTF_Font* ttf_font,
    const char* text,
    const std::size_t text_length,
    const int max_width_pixels,
    int *const measured_width_pixels,
    std::size_t *const measured_length
) {
    // if (!TTF_SetTextString(ttf_text, text.data(), text.size())) {
    //     throw std::runtime_error("TTF_SetTextString failure");
    // }
    // int w = 0;
    // int h = 0;
    // if (!TTF_GetTextSize(ttf_text, &w, &h)) {
    //     throw std::runtime_error("TTF_GetTextSize failure");
    // }
    // return std::make_tuple(w, h);

    if (!TTF_MeasureString(
            ttf_font,
            text,
            text_length,
            max_width_pixels,
            measured_width_pixels,
            measured_length
        )
    ) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("measure string failed: {}", error);
        return false;
    }

    return true;
}

// TODO: this may error if a very, very long line is passed!
// Need to limit line size to max 10 * 4096 characters or something
bool calculate_number_of_characters_that_fit_on_line(
    TTF_Font* ttf_font,
    const std::string& line,
    const std::size_t pos,
    const int text_area_width_in_pixels,
    int *const prefix_width_in_pixels,
    std::size_t *const prefix_length
) {

    //int prefix_width_in_pixels{0};
    //int prefix_height_in_pixels{0};
    //std::size_t prefix_length{0};

    const char* text = line.data() + pos;
    const std::size_t text_length = line.size() - pos;

    // Convert from line and start position to a string_view over the remaining
    // part of the line
    const auto line_view{std::string_view(text, text_length)};
    SPDLOG_DEBUG("line_view={}", line_view);

    if (!calculate_text_width_in_pixels_and_length(
        ttf_font,
        text,
        text_length,
        text_area_width_in_pixels,
        prefix_width_in_pixels,
        prefix_length
    )) {
        return false;
    }

    SPDLOG_DEBUG("calculate_number_of_characters_that_fit_on_line: calculated prefix_length={}", *prefix_length);

    return true;
}

std::size_t find_wrap_position(
    const std::string& line,
    const std::size_t end_pos
) {
    // TODO: currently does nothing !
    // Wrapping is disabled
    return end_pos;
}

std::vector<TextLayoutEngine::DocumentLayoutLine> wrap_line(
    TTF_Font* ttf_font,
    const int font_line_skip,
    const std::string& line,
    const std::size_t line_index,
    const int text_area_width_in_pixels,
    int &y,
    const int dy /* same as font_line_skip, but in principle may be different */
) {

    std::vector<TextLayoutEngine::DocumentLayoutLine> document_layout_lines;

    std::size_t pos{0};

    while (pos <= line.length()) {

        int prefix_width_in_pixels{0};
        std::size_t prefix_length{0};

        if (!calculate_number_of_characters_that_fit_on_line(
                ttf_font,
                line,
                pos,
                text_area_width_in_pixels,
                &prefix_width_in_pixels,
                &prefix_length
            )
        ) {
            // ERROR: what to do here?
            break;
        };

        const auto number_of_characters = prefix_length;

        auto end_pos{pos + number_of_characters};
        SPDLOG_DEBUG("end_pos calculated to be {}", end_pos);

        // Can't fit a single character, or line has length 0
        if (end_pos == pos) {
            if (line.size() == 0) {
                SPDLOG_DEBUG("line length is 0");
            }
            else {
                SPDLOG_DEBUG("can't fit a single character!");
            }
            end_pos = std::min(pos + 1, line.length());
        }

        if (end_pos < line.length()) {
            const auto wrap_pos = find_wrap_position(line, end_pos);
            SPDLOG_DEBUG("wrap_pos={}", wrap_pos);

            // If the first character is a space, and this is the only space
            // then wrap_pos == pos
            if (wrap_pos > pos) {
                SPDLOG_DEBUG("wrap_pos > pos, setting end_pos=wrap_pos={}", wrap_pos);
                end_pos = wrap_pos;
            }
        }

        const auto text_span{
            std::string_view(
                line.data() + pos,
                line.data() + end_pos
            )
        };
        SPDLOG_DEBUG("text_span={}", text_span);

        // TODO: There was part of the algorithm here to trim leading spaces
        // but I don't think that is correct for a programming text editor.

        const auto document_layout_line{
            TextLayoutEngine::DocumentLayoutLine{
                .line_index{line_index},
                .column_index{pos},
                .text_span{text_span},
                .x{0},
                .y{y},
                .w{prefix_width_in_pixels},
                .h{font_line_skip}
            }
        };

        document_layout_lines.push_back(std::move(document_layout_line));

        pos = end_pos;
        SPDLOG_DEBUG("pos advanced to end_pos = {}", end_pos);

        y += dy;

        if (pos == line.length()) {
            SPDLOG_DEBUG("reached end of line, break");
            break;
        }
    }

    return document_layout_lines;
}

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

TextLayoutEngine::DocumentLayoutCursorPosition TextLayoutEngine::convert_document_cursor_position_to_document_layout_cursor_position(
    TTF_Font* ttf_font,
    const int font_line_skip,
    const DocumentLayout& document_layout,
    const DocumentCursorPosition& document_cursor_position,
    const int text_area_width_in_pixels
) {

    SPDLOG_INFO("start of cursor position algorithm");
    SPDLOG_INFO("document_cursor_position={}", std::format("{}", document_cursor_position));

    int y{0};
    const int dy{font_line_skip};

    for (const auto& [i, document_layout_line]: std::ranges::enumerate_view(document_layout.lines)) {
        if (document_cursor_position.line_index == document_layout_line.line_index) {
            const auto text_span{document_layout_line.text_span};
            const auto text_span_length = text_span.size();

            SPDLOG_INFO("i={}", i);

            if (document_cursor_position.column_index >= document_layout_line.column_index &&
                document_cursor_position.column_index <= document_layout_line.column_index + text_span_length)
                // Comparison is <= because cursor must be able to move 1 position beyond end of line
            {

                const auto column_index{
                    document_cursor_position.column_index - document_layout_line.column_index
                };
                SPDLOG_INFO("!!! check this: column_index={}", column_index);

                int w{0};
                std::string tmp_text = std::string(
                    text_span.substr(column_index, 1)
                );

                // This is a bit of a hack to handle the case where the cursor is 1 position past the end
                if (tmp_text.size() < 1) {
                    tmp_text = "_";
                }

                SPDLOG_INFO("tmp_text={}", tmp_text);
                if (
                    !calculate_text_width_in_pixels_and_length(
                        ttf_font,
                        tmp_text.c_str(),
                        tmp_text.size(),
                        text_area_width_in_pixels,
                        &w,
                        nullptr
                    )
                ) {
                    // TODO
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
                        // TODO
                    }
                }

                TextLayoutEngine::DocumentLayoutCursorPosition document_layout_cursor_position(
                    static_cast<std::size_t>(i),
                    column_index,
                    x,
                    y,
                    w,
                    font_line_skip
                );

                return document_layout_cursor_position;
            }
        }

        y += dy;
    }

    return TextLayoutEngine::DocumentLayoutCursorPosition();
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
    const DocumentLayoutCursorPosition& document_layout_cursor_position
) {

    const auto x{document_layout_cursor_position.x};
    const auto y{document_layout_cursor_position.y};
    const auto w{document_layout_cursor_position.w};
    const auto h{document_layout_cursor_position.h};

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