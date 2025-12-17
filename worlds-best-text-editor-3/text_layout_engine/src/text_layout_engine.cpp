#include "spdlog_util.hpp"

#include "text_layout_engine.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
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

TextLayoutEngine::DocumentLayout TextLayoutEngine::create_document_layout(
    const Document& document,
    TTF_Text* ttf_text,
    const int width_in_pixels
) {
    DocumentLayout document_layout;

    int counter = 0;

    std::size_t line_index = 0;
    for (const auto& line: document.lines) {
        SPDLOG_INFO("line_index={}", line_index);

        auto document_layout_line = DocumentLayoutLine();
        document_layout_line.line_index = line_index;

        if (line.size() == 0) {
            const auto text_span = std::string_view(line.data(), 0);
            SPDLOG_INFO("adding line: '{}'", text_span);
            document_layout_line.text_span = text_span;
            document_layout.lines.push_back(document_layout_line);
        }
        // TODO: may be able to remove this if statement by making the condition
        // in the while statement <= rather than <
        else {
            std::size_t start = 0;
            while (start < line.size()) {

                //SDL_Delay(1000);
                counter ++;
                if (counter > 50) {
                    return document_layout;
                    //std::exit(0);
                }

                const std::size_t wrap_length = calculate_wrap_length(
                    ttf_text,
                    width_in_pixels,
                    line,
                    start
                );

                const auto text_span = std::string_view(
                    line.data() + start,
                    wrap_length
                );

                SPDLOG_INFO("adding line: '{}'", text_span);

                document_layout_line.text_span = text_span;
                document_layout.lines.push_back(document_layout_line);

                start += wrap_length;
                SPDLOG_INFO("start now set to start={}", start);
            }
        }

        ++ line_index;
    }

    return document_layout;
}