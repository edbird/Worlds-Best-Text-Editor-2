#ifndef TEXT_AREA_HPP
#define TEXT_AREA_HPP

#include "text_layout_engine_measure_string.hpp"
#include "window_geometry.hpp"
#include "document.hpp"

using namespace TextLayoutEngine;

struct GUIObject {

    virtual ~GUIObject() {}

    virtual void draw() const = 0;

    virtual void frame_update() = 0;
};

struct TextArea : public GUIObject {

    TextArea(
        TTF_Text* ttf_text,
        const int font_line_skip,
        const int screen_width_in_pixels,
        const int screen_height_in_pixels
    )
        : start_line{0}
        , ttf_text{ttf_text}
        , font_line_skip{font_line_skip}
        , screen_width_in_pixels{screen_width_in_pixels}
        , screen_height_in_pixels{screen_height_in_pixels}
        , frame_count{0}
    {
    }

    virtual ~TextArea();

    virtual void draw() const;

    virtual void frame_update();

    void update_document(
        const Document& document,
        const WindowGeometry& window_geometry
    );

    void log_rendering_result();

    DocumentLayout document_layout;
    std::size_t start_line;
    TTF_Text* ttf_text;
    int font_line_skip;
    int screen_width_in_pixels;
    int screen_height_in_pixels;

    // TODO: remove
    int64_t frame_count;
};

#endif