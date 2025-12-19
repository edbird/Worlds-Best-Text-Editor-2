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

    virtual void draw() const = 0;

    virtual void handle_command(Command &command) = 0;

    virtual void frame_update() = 0;
};

struct TextArea : public GUIObject {

    TextArea(
        TTF_Text* ttf_text,
        const int font_line_skip,
        const int width_in_pixels,
        const int height_in_pixels,
        const std::optional<std::reference_wrapper<Document>> optional_document
    )
        : start_line{0}
        , ttf_text{ttf_text}
        , font_line_skip{font_line_skip}
        , width_in_pixels{width_in_pixels}
        , height_in_pixels{height_in_pixels}
        , frame_count{0}
        , optional_document{optional_document}
    {
    }

    virtual ~TextArea();

    virtual void draw() const;

    virtual void handle_command(Command& command);

    virtual void frame_update();

    void update_geometry(const int width, const int height) {
        width_in_pixels = width;
        height_in_pixels = height;
    }

    void update_document_layout();

    void log_rendering_result();

    DocumentLayout document_layout;
    std::size_t start_line;
    TTF_Text* ttf_text;
    int font_line_skip;
    int width_in_pixels;
    int height_in_pixels;
    std::optional<std::reference_wrapper<Document>> optional_document;

    // TODO: remove
    int64_t frame_count;
};

#endif