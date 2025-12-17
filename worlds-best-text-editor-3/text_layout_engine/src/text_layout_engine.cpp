#include "spdlog_util.hpp"

#include "text_layout_engine.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <tuple>

std::tuple<int, int> calculate_text_width_and_height_in_pixels(
    TTF_Text* ttf_text,
    std::string_view text
) {
    if (!TTF_SetTextString(ttf_text, text.data(), text.size())) {
        throw std::runtime_error("TTF_SetTextString failure");
    }
    int w = 0;
    int h = 0;
    if (!TTF_GetTextSize(ttf_text, &w, &h)) {
        throw std::runtime_error("TTF_GetTextSize failure");
    }
    return std::make_tuple(w, h);
}

std::tuple<std::size_t, int, int> calculate_number_of_characters_that_fit_on_line(
    TTF_Text* ttf_text,
    const std::string& line,
    const std::size_t pos,
    const std::size_t width_in_pixels
) {

    // Convert from line and start position to a string_view over the remaining
    // part of the line
    const auto line_view{std::string_view(line.data() + pos, line.data() + line.size())};
    SPDLOG_DEBUG("line_view={}", line_view);

    // Create iterator over range of index values of `line_view`
    const auto index_it = std::views::iota(
        static_cast<decltype(line_view.size())>(0),
        line_view.size()
    );

    int prefix_width_in_pixels{0};
    int prefix_height_in_pixels{0};

    const auto lambda = [
        ttf_text,
        line_view,
        &prefix_width_in_pixels,
        &prefix_height_in_pixels
    ](const auto end_index, const auto& width_in_pixels) -> bool {
        SPDLOG_DEBUG("lambda: end_index={}, ", end_index);

        // Convert end index to substring (prefix) view
        const auto prefix_view{std::string_view(line_view.data(), line_view.data() + end_index)};
        SPDLOG_DEBUG("testing prefix: {}", prefix_view);

        // Convert the prefix to length in pixels
        const auto [_prefix_width_in_pixels, _prefix_height_in_pixels]{
            calculate_text_width_and_height_in_pixels(ttf_text, prefix_view)
        };
        SPDLOG_DEBUG("prefix_width_in_pixels={}", prefix_width_in_pixels);

        prefix_width_in_pixels = _prefix_width_in_pixels;
        prefix_height_in_pixels = _prefix_height_in_pixels;

        // TODO: check <= here or < ???
        return _prefix_width_in_pixels < static_cast<int64_t>(width_in_pixels);
    };

    const auto end_it = std::lower_bound(
        index_it.begin(),
        index_it.end(),
        width_in_pixels,
        lambda
    );
    const auto end_index{*end_it};
    SPDLOG_DEBUG("calculate_number_of_characters_that_fit_on_line: calculated end_index={}", end_index);

    return std::make_tuple(end_index, prefix_width_in_pixels, prefix_height_in_pixels);
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
    TTF_Text* ttf_text,
    const std::string& line,
    const std::size_t line_index,
    std::size_t width_in_pixels
) {

    std::vector<TextLayoutEngine::DocumentLayoutLine> document_layout_lines;

    std::size_t pos{0};

    while (pos <= line.length()) {

        const auto [number_of_characters, width_pixels, height_pixels]{
            calculate_number_of_characters_that_fit_on_line(ttf_text, line, pos, width_in_pixels)
        };

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
                .width_pixels{width_pixels},
                .height_pixels{height_pixels},
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
    TTF_Text* ttf_text,
    const Document& document,
    const std::size_t width_in_pixels
) {
    DocumentLayout document_layout;

    int counter = 0;

    std::size_t line_index = 0;
    for (const auto& line: document.lines) {
        SPDLOG_DEBUG("line_index={}", line_index);

        std::vector<DocumentLayoutLine> wrapped_lines = wrap_line(ttf_text, line, line_index, width_in_pixels);
        document_layout.lines.append_range(std::move(wrapped_lines));

        ++ line_index;
    }

    return document_layout;
}