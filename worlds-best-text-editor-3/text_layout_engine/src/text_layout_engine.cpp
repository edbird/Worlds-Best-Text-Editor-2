#include "spdlog_util.hpp"

#include "text_layout_engine.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>

std::size_t TextLayoutEngine::calculate_wrap_length(
    TTF_Text* ttf_text,
    const int width_in_pixels,
    const std::string& line,
    const std::size_t start
) {

    SPDLOG_INFO("calculate_wrap_length: line:");
    SPDLOG_INFO("{}", line);

    // Find the maximum number of characters which can be rendered within the
    // available space

    const auto lambda = [&line, &start, &ttf_text](const auto& index, const auto &width_in_pixels) -> bool {
        SPDLOG_INFO("lambda called: index={}, width_in_pixels={}", index, width_in_pixels);

        const auto substring = line.substr(start, index - start);

        if (!TTF_SetTextString(ttf_text, substring.c_str(), substring.size())) {
            throw std::runtime_error("TTF_SetTextString failure");
        }
        int w = 0;
        int h = 0;
        if (!TTF_GetTextSize(ttf_text, &w, &h)) {
            throw std::runtime_error("TTF_GetTextSize failure");
        }

        // TODO: check <= here or < ???
        return w <= static_cast<int64_t>(width_in_pixels);
    };

    const auto integer_range_it = std::views::iota(start, line.size());
    const auto end_it = std::lower_bound(
        integer_range_it.begin(),
        integer_range_it.end(),
        static_cast<std::size_t>(width_in_pixels),
        lambda
    );
    const auto end = *end_it;

    // At this point, the end index is known

    // Is it the end of the line?
    if (end == line.size()) {
        SPDLOG_INFO("end of the line");
        return end - start;
    }
    // Is the next character a space?
    else if (line[end + 1] == ' ') {
        SPDLOG_INFO("not the end of the line, but space follows as next character");
        return end - start;
    }
    // Not the end of the line, not followed by a space
    else {
        SPDLOG_INFO("not the end of the line, need to word-wrap");
        // Need to word-wrap. Is there a space (word boundary) anywhere on this
        // line?
    }

    const auto substring_view = std::string_view(
        line.data() + start,
        line.data() + (end - start)
    );
    const auto find_pos = substring_view.find_last_of(' ');

    if (find_pos == std::string::npos) {
        // not found: the entire line is a single word or part of a single word
        SPDLOG_INFO("calculate_wrap_length: end of word not found, returning end={}, start={}, end-start={}", end, start, end-start);
        return end - start;
    }
    else {
        // found
        SPDLOG_INFO("calculate_wrap_length: end of word found at position {}, returning start={}, find_pos-start={}", find_pos, start, find_pos-start);
        return find_pos - start;
    }
}

std::size_t calculate_text_width_in_pixels(
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
    return static_cast<std::size_t>(w);
}

std::size_t calculate_number_of_characters_that_fit_on_line(
    TTF_Text* ttf_text,
    const std::string& line,
    const std::size_t pos,
    const std::size_t width_in_pixels
) {

    // Convert from line and start position to a string_view over the remaining
    // part of the line
    const auto line_view{std::string_view(line.data() + pos, line.data() + line.size())};
    SPDLOG_INFO("line_view={}", line_view);

    // Create iterator over range of index values of `line_view`
    const auto index_it = std::views::iota(
        static_cast<decltype(line_view.size())>(0),
        line_view.size()
    );

    const auto lambda = [ttf_text, line_view](const auto end_index, const auto& width_in_pixels) -> bool {
        SPDLOG_INFO("lambda: end_index={}, ", end_index);

        // Convert end index to substring (prefix) view
        const auto prefix_view{std::string_view(line_view.data(), line_view.data() + end_index)};
        SPDLOG_INFO("testing prefix: {}", prefix_view);

        // Convert the prefix to length in pixels
        const auto prefix_width_in_pixels{calculate_text_width_in_pixels(ttf_text, prefix_view)};
        SPDLOG_INFO("prefix_width_in_pixels={}", prefix_width_in_pixels);

        // TODO: check <= here or < ???
        return prefix_width_in_pixels < static_cast<int64_t>(width_in_pixels);
    };

    const auto end_it = std::lower_bound(
        index_it.begin(),
        index_it.end(),
        width_in_pixels,
        lambda
    );
    const auto end_index{*end_it};
    SPDLOG_INFO("calculate_number_of_characters_that_fit_on_line: calculated end_index={}", end_index);

    return end_index;
}

std::size_t find_wrap_position(
    const std::string& line,
    const std::size_t end_pos
) {
    // TODO: currently does nothing !
    // Wrapping is disabled
    return end_pos;
}

std::vector<TextLayoutEngine::DocumentLayoutLine> TextLayoutEngine::wrap_line(
    TTF_Text* ttf_text,
    const std::string& line,
    const std::size_t line_index,
    std::size_t width_in_pixels
) {

    std::vector<DocumentLayoutLine> document_layout_lines;

    std::size_t pos{0};
    //std::size_t char_index{0}; // this is always the same as pos

    // TODO: I modified this to try and work with length 0 lines
    while (pos <= line.length()) {

        auto end_pos{
            calculate_number_of_characters_that_fit_on_line(ttf_text, line, pos, width_in_pixels)
        };
        SPDLOG_INFO("end_pos calculated to be {}", end_pos);

        // Can't fit a single character, or line has length 0
        if (end_pos == pos) {
            if (line.size() == 0) {
                SPDLOG_INFO("line length is 0");
            }
            else {
                SPDLOG_INFO("can't fit a single character!");
            }
            end_pos = std::min(pos + 1, line.length());
        }

        if (end_pos < line.length()) {
            const auto wrap_pos = find_wrap_position(line, end_pos);
            SPDLOG_INFO("wrap_pos={}", wrap_pos);

            // If the first character is a space, and this is the only space
            // then wrap_pos == pos
            if (wrap_pos > pos) {
                SPDLOG_INFO("wrap_pos > pos, setting end_pos=wrap_pos={}", wrap_pos);
                end_pos = wrap_pos;
            }
        }

        const auto text_span{
            std::string_view(
                line.data() + pos,
                line.data() + end_pos
            )
        };
        SPDLOG_INFO("text_span={}", text_span);

        // TODO: There was part of the algorithm here to trim leading spaces
        // but I don't think that is correct for a programming text editor.

        const auto document_layout_line{
            DocumentLayoutLine{
                .line_index{line_index},
                //.char_index{char_index},
                .char_index{pos},
                .text_span{text_span},
            }
        };

        // TODO: cache width and height in pixels?

        document_layout_lines.push_back(std::move(document_layout_line));

        pos = end_pos;
        SPDLOG_INFO("pos advanced to end_pos = {}", end_pos);
        //char_index = pos;
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
        SPDLOG_INFO("line_index={}", line_index);

        auto document_layout_line = DocumentLayoutLine();
        document_layout_line.line_index = line_index;

        std::vector<DocumentLayoutLine> wrapped_lines = wrap_line(ttf_text, line, line_index, width_in_pixels);
        document_layout.lines.append_range(
            std::make_move_iterator(wrapped_lines)
        );

        ++ line_index;
    }

        // if (line.size() == 0) {
        //     const auto text_span = std::string_view(line.data(), 0);
        //     SPDLOG_INFO("adding line: '{}'", text_span);
        //     document_layout_line.text_span = text_span;
        //     document_layout.lines.push_back(document_layout_line);
        // }
        // // TODO: may be able to remove this if statement by making the condition
        // // in the while statement <= rather than <
        // else {
        //     std::size_t start = 0;
        //     while (start < line.size()) {

        //         //SDL_Delay(1000);
        //         counter ++;
        //         if (counter > 50) {
        //             return document_layout;
        //             //std::exit(0);
        //         }

        //         const std::size_t wrap_length = calculate_wrap_length(
        //             ttf_text,
        //             width_in_pixels,
        //             line,
        //             start
        //         );

        //         const auto text_span = std::string_view(
        //             line.data() + start,
        //             wrap_length
        //         );

        //         SPDLOG_INFO("adding line: '{}'", text_span);

        //         document_layout_line.text_span = text_span;
        //         document_layout.lines.push_back(document_layout_line);

        //         start += wrap_length;
        //         SPDLOG_INFO("start now set to start={}", start);
        //     }
        // }

        // ++ line_index;

    return document_layout;
}