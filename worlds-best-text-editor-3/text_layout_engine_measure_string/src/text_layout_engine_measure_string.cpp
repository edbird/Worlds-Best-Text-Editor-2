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
    const std::string& line,
    const std::size_t line_index,
    const int text_area_width_in_pixels
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
                .char_index{pos},
                .text_span{text_span},
                .width_pixels{prefix_width_in_pixels},
                .height_pixels{0}, // TODO: now not used
            }
        };

        document_layout_lines.push_back(std::move(document_layout_line));

        pos = end_pos;
        SPDLOG_DEBUG("pos advanced to end_pos = {}", end_pos);

        if (pos == line.length()) {
            SPDLOG_DEBUG("reached end of line, break");
            break;
        }
    }

    return document_layout_lines;
}

TextLayoutEngine::DocumentLayout TextLayoutEngine::create_document_layout(
    TTF_Font* ttf_font,
    const std::optional<std::reference_wrapper<Document>> optional_document,
    const int text_area_width_in_pixels
) {
    DocumentLayout document_layout;
    // layout cursor: (layout_row, layout_col)
    std::get<0>(document_layout.cursor) = 0;
    std::get<1>(document_layout.cursor) = 0;

    if (optional_document.has_value()) {
        auto &document{optional_document.value().get()};

        std::size_t line_index = 0;
        for (const auto& line: document.lines) {
            SPDLOG_DEBUG("line_index={}", line_index);

            std::vector<DocumentLayoutLine> wrapped_lines = wrap_line(ttf_font, line, line_index, text_area_width_in_pixels);
            document_layout.lines.append_range(std::move(wrapped_lines));

            ++ line_index;
        }

        auto [doc_cursor_y, doc_cursor_x]{document.cursor};

        std::size_t accumulated_chars{0};
        std::size_t layout_row{0};

        for (const auto& layout_line: document_layout.lines) {
            if (layout_line.line_index == doc_cursor_y) {
                const std::size_t span_len = layout_line.text_span.size();

                if (doc_cursor_x >= accumulated_chars + span_len) {
                    // cursor is after this wrapped span; accumulate and continue
                    accumulated_chars += span_len;
                }
                else {
                    // cursor lies within this wrapped span
                    std::get<0>(document_layout.cursor) = layout_row;
                    std::get<1>(document_layout.cursor) = doc_cursor_x - accumulated_chars;
                    break;
                }
            }

            ++layout_row;
        }
    }

    const auto [cursor_y, cursor_x]{document_layout.cursor};
    SPDLOG_INFO("cursor position: {}, {}", cursor_y, cursor_x);

    return document_layout;
}

bool TextLayoutEngine::draw_document_layout(
    SDL_Renderer* renderer,
    TTF_Text* ttf_text,
    const int font_line_skip,
    const int text_area_width_in_pixels,
    const int text_area_height_in_pixels,
    const DocumentLayout& document_layout,
    const std::size_t start_line
) {
    const auto [cursor_y, cursor_x]{document_layout.cursor};

    int y = 0;
    const auto dy{font_line_skip};
    for (const auto& [line_index, line]: std::ranges::enumerate_view(document_layout.lines)) {

        if (line_index < static_cast<int64_t>(start_line)) {
            continue;
        }

        const auto line_width_in_pixels = line.width_pixels;
        //const auto height_pixels = line.height_pixels;
        const auto line_height_in_pixels = font_line_skip;

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
        if (!TTF_DrawRendererText(ttf_text, 0.0f, static_cast<float>(y))) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("failed to draw text: {}", error);
            return false;
        }

        if (line_index == cursor_y) {
            // TODO: check cursor_x < actual length?
            int measured_width{0};
            const auto ttf_font = TTF_GetTextFont(ttf_text);
            if (!TTF_MeasureString(ttf_font, line_text.data(), cursor_x, text_area_width_in_pixels, &measured_width, nullptr)) {
                const auto error = SDL_GetError();
                SPDLOG_ERROR("TTF_MeasureString error: {}", error);
                return false;
            }
            // Required because length = 0 means null-terminated string
            if (cursor_x == 0) {
                measured_width = 0;
            }
            //SPDLOG_INFO("cursor x placement: measured_width={}", measured_width);

            const auto y{font_line_skip * cursor_y};
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
        }

        y += dy;
    }

    return true;
}