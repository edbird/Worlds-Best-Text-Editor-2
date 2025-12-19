#include "spdlog_util.hpp"

#include "performance_util.hpp"
#include "application_configuration.hpp"
#include "window_geometry.hpp"
#include "application_resources.hpp"
#include "color_util.hpp"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_version.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
// #include <SDL3/SDL_render.h>
// #include <SDL3/SDL_error.h>
// #include <SDL3/SDL_init.h>

#include <cstdint>
#include <memory>
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


struct AppState {

    AppState(
        std::unique_ptr<WindowGeometry> window_geometry
    )
        : gui_objects{std::vector<std::unique_ptr<GUIObject>>()}
        , application_resources{std::make_unique<ApplicationResources>()}
        , window_geometry{std::move(window_geometry)}
        , frame_counter{std::make_unique<FrameCounter>()}
        , performance_timer{std::make_unique<PerformanceTimer>()}
    {

    }

    AppState(const AppState& other) = delete;
    AppState(AppState&& other) = delete;
    AppState& operator=(const AppState& other) = delete;
    AppState& operator=(AppState&& other) = delete;

    std::vector<std::unique_ptr<GUIObject>> gui_objects;
    std::unique_ptr<ApplicationResources> application_resources;
    std::unique_ptr<WindowGeometry> window_geometry;
    std::unique_ptr<FrameCounter> frame_counter;
    std::unique_ptr<PerformanceTimer> performance_timer;
    Document document;
};


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SPDLOG_INFO("SDL_AppInit");

    auto app_state = new AppState(
        std::make_unique<WindowGeometry>()
    );
    *appstate = static_cast<void*>(app_state);

    // TODO: no longer required?
    ApplicationResources& application_resources = *(app_state->application_resources);

    SPDLOG_INFO("Worlds Best Text Editor startup");

    const auto maybe_application_configuration{initialize_application_configuration()};
    if (!maybe_application_configuration) {
        SPDLOG_ERROR("failed to initialize application configuration");
        return SDL_APP_FAILURE;
    }
    const auto application_configuration{maybe_application_configuration.value()};

    const auto optional_font_path{application_configuration.getFontPath()};
    if (!optional_font_path) {
        SPDLOG_ERROR("failed to get font path from application configuration");
        return SDL_APP_FAILURE;
    }
    const auto font_path{optional_font_path.value()};

    const auto optional_font_size{application_configuration.getFontSize()};
    if (!optional_font_size) {
        SPDLOG_ERROR("failed to get font size from application configuration");
        return SDL_APP_FAILURE;
    }
    const auto font_size{optional_font_size.value()};

    // not sure if needed?
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");

    query_sdl_version();
    query_sdl_performance_counter_frequency();

    if (!init_application_metadata()) {
        return SDL_APP_FAILURE;
    }

    query_video_drivers();
    query_render_drivers();

    if (!initialize_sdl(application_resources)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }

    if (!initialize_sdl_ttf(application_resources)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }

    const WindowGeometry& window_geometry = *(app_state->window_geometry);

    if (!initialize_window(application_resources, window_geometry)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }
    const auto window = application_resources.window_list.front();

    if (!initialize_renderer(application_resources, window)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }
    const auto renderer = application_resources.renderer_list.front();

    if (!query_renderer_name(renderer)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }

    if (!initialize_text_engine(application_resources, renderer)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }
    const auto text_engine = application_resources.text_engine_list.front();

    if (!initialize_ttf_font(application_resources, font_path.c_str(), font_size)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }
    const auto ttf_font = application_resources.ttf_font_list.front();
    const auto font_line_skip{TTF_GetFontLineSkip(ttf_font)};

    // Maybe should be managed by TextArea?
    if (!initialize_ttf_text(application_resources, text_engine, ttf_font)) {
        cleanup(application_resources);
        return SDL_APP_FAILURE;
    }
    const auto ttf_text = application_resources.ttf_text_list.front();

    Document& document{app_state->document};
    document.read_from_file("example_textfile.txt");

    auto text_area{
        std::make_unique<TextArea>(
            ttf_text,
            font_line_skip,
            window_geometry.screen_width_in_pixels,
            window_geometry.screen_height_in_pixels
        )
    };

    text_area->update_document(document, window_geometry);

    auto &gui_objects = app_state->gui_objects;
    gui_objects.push_back(std::move(text_area));

    const auto performance_counter_frequency = SDL_GetPerformanceFrequency();
    auto performance_counter_last = SDL_GetPerformanceCounter();

    app_state->frame_counter = std::move(std::make_unique<FrameCounter>());
    app_state->performance_timer = std::move(
        std::make_unique<PerformanceTimer>(
            performance_counter_last,
            performance_counter_frequency
        )
    );

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    //SPDLOG_INFO("SDL_AppIterate");

    static Uint64 last_tick = 0;
    const Uint64 current_tick = SDL_GetTicksNS();
    const Uint64 target_ns = 1000000000 / 60; // 60 FPS
    if (current_tick - last_tick < target_ns) {
        return SDL_APP_CONTINUE;
    }
    last_tick = current_tick;

    const auto app_state = reinterpret_cast<AppState*>(appstate);

    ApplicationResources& application_resources = *(app_state->application_resources);

    // TODO: don't like this - it is confusing since these are vectors but can only really have a single element
    const auto renderer = application_resources.renderer_list.front();

    FrameCounter& frame_counter = *(app_state->frame_counter);
    PerformanceTimer& performance_timer = *(app_state->performance_timer);

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

    auto &gui_objects = app_state->gui_objects;
    for (auto &gui_object: gui_objects) {
        gui_object->draw();
    }

    if (!SDL_RenderPresent(renderer)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("render present failed: {}", error);
    }

    increment_frame_counter(frame_counter);
    const auto performance_counter = SDL_GetPerformanceCounter();
    const auto time_in_seconds{get_time_elapsed_in_seconds(performance_timer, performance_counter)};
    if (time_in_seconds >= 10.0) {
        const auto frame_rate{get_frame_rate(frame_counter, performance_timer, performance_counter)};
        const auto frame_count{frame_counter.frame_count};
        SPDLOG_INFO("performance metrics: {} frames in 10 seconds, {} frames/sec", frame_count, frame_rate);
        update_performance_timer(performance_timer, performance_counter);
        reset_frame_counter(frame_counter);
    }

    for (auto &gui_object: gui_objects) {
        gui_object->frame_update();
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *p_event) {
    //SPDLOG_INFO("SDL_AppEvent");

    const auto app_state = reinterpret_cast<AppState*>(appstate);

    ApplicationResources& application_resources = *(app_state->application_resources);

    // TODO: don't like this - it is confusing since these are vectors but can only really have a single element
    //const auto window = application_resources.window_list.front();

    const SDL_Event& event{*p_event};

    if (event.type == SDL_EVENT_QUIT) {
        SPDLOG_INFO("SDL_EVENT_QUIT");
        return SDL_APP_SUCCESS;
    }

    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
        SPDLOG_INFO("SDL_EVENT_WINDOW_CLOSE_REQUESTED");
        return SDL_APP_SUCCESS;
    }

    if (event.type == SDL_EVENT_WINDOW_RESIZED) {
        const int w{event.window.data1};
        const int h{event.window.data2};
        SPDLOG_INFO("SDL_EVENT_WINDOW_RESIZED: geometry {}, {}", w, h);

        auto &window_geometry{app_state->window_geometry};
        window_geometry->update_geometry(w, h);

        // TODO: need some kind of hierachical container system to place the text area inside
        // some other gui object, which contains also things like status bar etc
        TextArea* const text_area = dynamic_cast<TextArea*>(app_state->gui_objects.front().get());
        if (!text_area) {
            SPDLOG_ERROR("dynamic cast failed");
        }
        else {
            Document& document{app_state->document};
            text_area->update_document(document, *window_geometry);
        }
    }

    if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
        const int w{event.window.data1};
        const int h{event.window.data2};
        SPDLOG_INFO("SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: geometry {}, {}", w, h);
    }

    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SPDLOG_INFO("SDL_AppQuit");

    const auto app_state = reinterpret_cast<AppState*>(appstate);

    ApplicationResources& application_resources = *(app_state->application_resources);

    cleanup(application_resources);

    delete app_state;
}