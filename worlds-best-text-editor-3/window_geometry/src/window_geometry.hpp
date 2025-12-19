#ifndef WINDOW_GEOMETRY_HPP
#define WINDOW_GEOMETRY_HPP

#include "spdlog_util.hpp"

struct WindowGeometry {

    WindowGeometry()
        : screen_width_in_pixels{800}
        , screen_height_in_pixels{600}
    {

    }

    void update_geometry(const int width, const int height) {
        screen_width_in_pixels = width;
        screen_height_in_pixels = height;
    }

    int screen_width_in_pixels;
    int screen_height_in_pixels;
};

#endif