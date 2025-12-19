#include "spdlog_util.hpp"

#include "text_layout_engine_measure_string.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <tuple>

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
    const std::size_t width_in_pixels,
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
        width_in_pixels,
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
    std::size_t width_in_pixels
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
                width_in_pixels,
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
    const Document& document,
    const std::size_t width_in_pixels
) {
    DocumentLayout document_layout;

    int counter = 0;

    std::size_t line_index = 0;
    for (const auto& line: document.lines) {
        SPDLOG_DEBUG("line_index={}", line_index);

        std::vector<DocumentLayoutLine> wrapped_lines = wrap_line(ttf_font, line, line_index, width_in_pixels);
        document_layout.lines.append_range(std::move(wrapped_lines));

        ++ line_index;
    }

    return document_layout;
}

bool TextLayoutEngine::draw_document_layout(
    TTF_Text* ttf_text,
    const int font_line_skip,
    const int screen_width_in_pixels,
    const int screen_height_in_pixels,
    const DocumentLayout& document_layout,
    const std::size_t start_line
) {
    int y = 0;
    const auto dy{font_line_skip};
    for (const auto& [line_index, line]: std::ranges::enumerate_view(document_layout.lines)) {

        if (line_index < start_line) {
            continue;
        }

        const auto width_pixels = line.width_pixels;
        //const auto height_pixels = line.height_pixels;
        const auto height_pixels = font_line_skip;

        if (0 + width_pixels <= screen_width_in_pixels) {

        }
        else {
            SPDLOG_WARN("skipping rendering of line {}, width is {} which exceeds maximum width of {}", line_index, 0 + width_pixels, screen_width_in_pixels);
            continue;
        }
        if (y + height_pixels <= screen_height_in_pixels) {

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
        y += dy;
    }

    return true;
}