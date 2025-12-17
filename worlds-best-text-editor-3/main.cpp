#include "spdlog_util.hpp"

#include "application_configuration.hpp"
#include "window_geometry.hpp"
#include "application_resources.hpp"
#include "color_util.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_version.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
// #include <SDL3/SDL_render.h>
// #include <SDL3/SDL_error.h>
// #include <SDL3/SDL_init.h>

#include <cstdint>
#include <print>
#include <format>

#include <ranges>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <json/json.h>

// TODO: change the video driver and renderer driver for windows

#include "document.hpp"
#include "text_layout_engine_measure_string.hpp"
#include "text_area.hpp"

using namespace TextLayoutEngine;

// TODO: move this, possibly simpler way to get the value
//const auto font_line_skip = TTF_GetFontLineSkip(TTF_GetTextFont(text_engine));



int main(int argc, char* argv[]) {

    // TODO: no longer required?
    auto application_resources = ApplicationResources();

    SPDLOG_INFO("Worlds Best Text Editor startup");

    const auto maybe_application_configuration{initialize_application_configuration()};
    if (!maybe_application_configuration) {
        SPDLOG_ERROR("failed to initialize application configuration");
        return -1;
    }
    const auto application_configuration{maybe_application_configuration.value()};

    const auto optional_font_path{application_configuration.getFontPath()};
    if (!optional_font_path) {
        SPDLOG_ERROR("failed to get font path from application configuration");
        return -1;
    }
    const auto font_path{optional_font_path.value()};

    const auto optional_font_size{application_configuration.getFontSize()};
    if (!optional_font_size) {
        SPDLOG_ERROR("failed to get font size from application configuration");
        return -1;
    }
    const auto font_size{optional_font_size.value()};

    // not sure if needed?
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");

    query_sdl_version();
    query_sdl_performance_counter_frequency();

    if (!init_application_metadata()) {
        return -1;
    }

    query_video_drivers();
    query_render_drivers();

    if (!initialize_sdl(application_resources)) {
        cleanup(application_resources);
        return -1;
    }

    if (!initialize_sdl_ttf(application_resources)) {
        cleanup(application_resources);
        return -1;
    }

    const auto window_geometry = WindowGeometry();

    if (!initialize_window(application_resources, window_geometry)) {
        cleanup(application_resources);
        return -1;
    }
    const auto window = application_resources.window_list.front();

    if (!initialize_renderer(application_resources, window)) {
        cleanup(application_resources);
        return -1;
    }
    const auto renderer = application_resources.renderer_list.front();

    if (!query_renderer_name(renderer)) {
        cleanup(application_resources);
        return -1;
    }

    if (!initialize_text_engine(application_resources, renderer)) {
        cleanup(application_resources);
        return -1;
    }
    const auto text_engine = application_resources.text_engine_list.front();

    if (!initialize_ttf_font(application_resources, font_path.c_str(), font_size)) {
        cleanup(application_resources);
        return -1;
    }
    const auto ttf_font = application_resources.ttf_font_list.front();
    const auto font_line_skip{TTF_GetFontLineSkip(ttf_font)};

    // TODO: add to ApplicationResources
    // Maybe should be managed by TextArea?
    TTF_Text* ttf_text = TTF_CreateText(text_engine, ttf_font, "", 0);

    Document document;
    document.read_from_file("example_textfile.txt");

    auto text_area{
        TextArea(
            ttf_text,
            font_line_skip,
            window_geometry.screen_width_in_pixels,
            window_geometry.screen_height_in_pixels
        )
    };

    text_area.update_document(document, window_geometry);

    auto gui_objects{std::vector<GUIObject*>()};
    gui_objects.push_back(&text_area);

    const auto frame_rate_latency = static_cast<Uint32>(1000.0 / 60.0);
    const auto performance_counter_frequency = SDL_GetPerformanceFrequency();
    const auto performance_counter_frequency_float = static_cast<double>(performance_counter_frequency);
    auto performance_counter_last = SDL_GetPerformanceCounter();
    uint64_t frame_count{0};
    for (bool exit = false; exit == false; ) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                SPDLOG_INFO("SDL_EVENT_QUIT");
                exit = true;
            }

            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                SPDLOG_INFO("SDL_EVENT_WINDOW_CLOSE_REQUESTED");
                exit = true;
            }
        }

        if (!SetRenderDrawColor(renderer, COLOR_BLACK)) {
            SPDLOG_ERROR("failed to set renderer drawing color");
        }

        if (!SDL_RenderClear(renderer)) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("render clear failed: {}", error);
        }

        if (!SetRenderDrawColor(renderer, COLOR_MAGENTA)) {
            SPDLOG_ERROR("failed to set renderer drawing color");
        }

        /*const auto ttf_font{TTF_GetTextFont(ttf_text)};
        if (!ttf_font) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("failed to get font from ttf text object: {}", error);
            return false;
        }

        const auto font_line_skip{TTF_GetFontLineSkip(ttf_font)};*/

        //TTF_DrawRendererText(ttf_text, 200, 200);
        /*draw_document_layout(
            ttf_text,
            font_line_skip,
            window_geometry.screen_width_in_pixels,
            window_geometry.screen_height_in_pixels,
            document_layout,
            text_area.start_line
        );*/

        for (auto &gui_object: gui_objects) {
            gui_object->draw();
        }

        if (!SDL_RenderPresent(renderer)) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("render present failed: {}", error);
        }

        //SDL_Delay(frame_rate_latency);
        SDL_WaitEventTimeout(nullptr, 16);

        ++ frame_count;
        const auto performance_counter = SDL_GetPerformanceCounter();
        const auto time_in_seconds = static_cast<double>(performance_counter - performance_counter_last) / performance_counter_frequency_float;
        if (time_in_seconds >= 10.0) {
            const auto frame_rate = static_cast<double>(frame_count) / time_in_seconds;
            SPDLOG_INFO("performance metrics: {} frames in 10 seconds, {} frames/sec", frame_count, frame_rate);
            performance_counter_last = performance_counter;
            frame_count = 0;
        }

        if (frame_count % 30 == 0) {
            text_area.start_line += 1;
        }
    }

    SPDLOG_INFO("destroy text");
    TTF_DestroyText(ttf_text);

    cleanup(application_resources);

    return 0;
}