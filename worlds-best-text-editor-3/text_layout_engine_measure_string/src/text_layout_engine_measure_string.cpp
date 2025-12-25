#include "spdlog_util.hpp"

#include "text_layout_engine_measure_string.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <tuple>
#include <functional>

#include "document_layout_cursor.hpp"
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

bool calculate_cursor_width(
    TTF_Font* const ttf_font,
    std::string text_under_cursor,
    const int text_area_width_in_pixels,
    int &w
) {

    // This is a bit of a hack to handle the case where the cursor is 1 position past the end
    if (text_under_cursor.size() < 1) {
        text_under_cursor = "_";
    }

    SPDLOG_INFO("text_under_cursor={}", text_under_cursor);
    SPDLOG_INFO("size: {}", text_under_cursor.size());
    if (
        !calculate_text_width_in_pixels_and_length(
            ttf_font,
            text_under_cursor.c_str(),
            text_under_cursor.size(),
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

TextLayoutEngine::DocumentLayoutCursor TextLayoutEngine::create_document_layout_cursor(
    TTF_Font* ttf_font,
    const int font_line_skip,
    const Document& document,
    const DocumentLayout& document_layout,
    const int text_area_width_in_pixels
) {
    // TODO: this is temporary, remove
    //SPDLOG_ERROR("should never reach this line");
    //return TextLayoutEngine::DocumentLayoutCursor();

    auto &document_cursor{document.document_cursor};

    SPDLOG_INFO("start of cursor position algorithm");
    SPDLOG_INFO("document_cursor={}", std::format("{}", document_cursor));

    const auto text_under_cursor{get_text_under_cursor(document)};
    SPDLOG_INFO("text_under_cursor={}", text_under_cursor);

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
                if (
                    !calculate_cursor_width(
                        ttf_font,
                        std::move(text_under_cursor),
                        text_area_width_in_pixels,
                        w
                    )
                ) {
                    SPDLOG_ERROR("failed to calculate cursor width");
                }
                SPDLOG_INFO("w={}", w);

                int x{0};

                std::string tmp_text = std::string(
                    text_span.substr(0, column_index)
                );

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
    if (!SDL_RenderRect(renderer, &rect)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to render document cursor: {}", error);
    };

    return true;
}
