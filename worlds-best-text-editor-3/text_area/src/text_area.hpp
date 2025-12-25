#ifndef TEXT_AREA_HPP
#define TEXT_AREA_HPP

#include "text_layout_engine_measure_string.hpp"
#include "window_geometry.hpp"
#include "document.hpp"
#include "command.hpp"

#include <functional>
#include <optional>

using namespace TextLayoutEngine;

struct GUIObject {

    virtual ~GUIObject() {}

    virtual const char* name() const = 0;

    virtual bool draw() const = 0;

    virtual void handle_command(Command &command) = 0;

    virtual void frame_update() = 0;
};

struct TextArea : public GUIObject {

    TextArea(
        SDL_Renderer* renderer,
        TTF_Text* ttf_text,
        const int width_in_pixels,
        const int height_in_pixels
    )
        : start_line{0}
        , renderer{renderer}
        , ttf_text{ttf_text}
        , ttf_font{nullptr}
        , font_line_skip{0}
        , width_in_pixels{width_in_pixels}
        , height_in_pixels{height_in_pixels}
        , frame_count{0}
        , document{Document()}
    {
        const auto ttf_font{TTF_GetTextFont(ttf_text)};
        if (!ttf_font) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("failed to get ttf font from ttf text: {}", error);
            throw std::runtime_error(
                std::format("failed to get ttf font from ttf text: {}", error)
            );
        }
        TextArea::ttf_font = ttf_font;
        font_line_skip = TTF_GetFontLineSkip(ttf_font);
    }

    virtual ~TextArea();

    virtual const char* name() const;

    virtual bool draw() const;

    virtual void handle_command(Command& command);

    virtual void frame_update();

    void update_geometry(const int width, const int height) {
        width_in_pixels = width;
        height_in_pixels = height;
    }

    void update_document_layout_and_cursor_position();
    void update_document_layout();
    void update_document_cursor_position();

    void log_rendering_result();

    DocumentLayout document_layout;
    std::size_t start_line;
    SDL_Renderer* renderer;
    TTF_Text* ttf_text;
    TTF_Font* ttf_font;
    int font_line_skip;
    int width_in_pixels;
    int height_in_pixels;
    Document document;

    // TODO: remove
    int64_t frame_count;
};

#endif