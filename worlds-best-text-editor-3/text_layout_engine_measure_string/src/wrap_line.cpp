#include "spdlog_util.hpp"

#include "wrap_line.hpp"

#include "document_layout_line.hpp"



bool calculate_text_width_in_pixels_and_length(
    TTF_Font* ttf_font,
    const char* text,
    const std::size_t text_length,
    const int max_width_pixels,
    int *const measured_width_pixels,
    std::size_t *const measured_length
) {
    SPDLOG_WARN("text_length={}", text_length);

    // if (!TTF_SetTextString(ttf_text, text.data(), text.size())) {
    //     throw std::runtime_error("TTF_SetTextString failure");
    // }
    // int w = 0;
    // int h = 0;
    // if (!TTF_GetTextSize(ttf_text, &w, &h)) {
    //     throw std::runtime_error("TTF_GetTextSize failure");
    // }
    // return std::make_tuple(w, h);

    // TTF_MeasureString interprets 0 length string parameter as meaning string
    // is null terminated and length should be calculated first.
    if (text_length == 0) {
        SPDLOG_INFO("text_length 0, return measured length 0");
        if (measured_width_pixels) {
            *measured_width_pixels = 0;
        }
        if (measured_length) {
            *measured_length = 0;
        }
        SPDLOG_INFO("return true");
        return true;

        // This comment moved from TextLayoutEngine
        //
        // Exceptional case for column 0 required because passing 0
        // for string length to TTF_MeasureString implies string
        // length determined by position of null in null terminated
        // string
    }

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
        SPDLOG_INFO("return false");
        return false;
    }

    SPDLOG_INFO("return true");
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
        SPDLOG_ERROR("failed to calculate number of characters that fit on line");
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