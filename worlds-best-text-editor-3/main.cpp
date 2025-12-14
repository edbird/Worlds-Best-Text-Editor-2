#include "spdlog_util.hpp"

#include "color_util.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_version.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_main.h>
// #include <SDL3/SDL_render.h>
// #include <SDL3/SDL_error.h>
// #include <SDL3/SDL_init.h>

#include <print>
#include <format>


/** Query video drivers
 */
void query_video_drivers() {

    std::vector<const char*> video_driver_list;
    for (int i = 0; i < SDL_GetNumVideoDrivers(); ++ i) {
        video_driver_list.push_back(SDL_GetVideoDriver(i));
    }

    std::string video_driver_list_string;
    const auto lambda = [&video_driver_list_string](const char* video_driver_list_next) {
        if (!video_driver_list_string.empty()) {
            video_driver_list_string += ", ";
        }
        video_driver_list_string += video_driver_list_next;
    };
    std::for_each(video_driver_list.cbegin(), video_driver_list.cend(), lambda);

    SPDLOG_INFO("Available video drivers: {}", video_driver_list_string);
}

/** Query render drivers
 */
void query_render_drivers() {

    std::vector<const char*> render_driver_list;
    for (int i = 0; i < SDL_GetNumRenderDrivers(); ++ i) {
        render_driver_list.push_back(SDL_GetRenderDriver(i));
    }

    std::string render_driver_list_string;
    const auto lambda = [&render_driver_list_string](const char* render_driver_list_next) {
        if (!render_driver_list_string.empty()) {
            render_driver_list_string += ", ";
        }
        render_driver_list_string += render_driver_list_next;
    };
    std::for_each(render_driver_list.cbegin(), render_driver_list.cend(), lambda);

    SPDLOG_INFO("Available render drivers: {}", render_driver_list_string);
}


int main(int argc, char* argv[]) {

    bool init_ok = true;

    SPDLOG_INFO("Worlds Best Text Editor startup");

    const auto sdl_version = SDL_VERSION;
    SPDLOG_INFO("SDL Version: {}", sdl_version);

    const char* app_name = "Worlds-Best-Text-Editor";
    const char *app_version = "v0.1.0";
    const char *app_identifier = "worldsbesttexteditor";
    const char *app_creator = "Edward Birdsall";
    const char *app_copyright = "All rights reserved";
    const char *app_url = "https://www.linkedin.com/in/edward-b-a71830218";
    const char *app_type = "application";

    if (!SDL_SetAppMetadata(app_name, app_version, app_identifier)) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metadata: {}", error);
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, app_creator)) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (creator): {}", error);
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, app_copyright)) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (copyright): {}", error);
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, app_url)) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (url): {}", error);
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, app_type)) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (): {}", error);
    }

    if (!init_ok) {
        return -1;
    }

    query_video_drivers();
    query_render_drivers();

    SPDLOG_INFO("SDL library initialize");
    const auto sdl_initflags = SDL_INIT_VIDEO;
    if (!SDL_Init(sdl_initflags)) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("SDL init failed: {}", error);
    }
    if (!init_ok) {
        return -1;
    }

    SPDLOG_INFO("SDL create window");
    const auto window_flags = SDL_WINDOW_RESIZABLE;
    const auto window = SDL_CreateWindow("Worlds Best Text Editor", 800, 600, window_flags);
    if (!window) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to create window: {}", error);
    }
    if (!init_ok) {
        return -1;
    }

    SPDLOG_INFO("SDL create renderer");
    SDL_Renderer *renderer = SDL_CreateRenderer(window, "gpu,opengl");
    if (!renderer) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to create renderer: {}", error);
    }
    if (!init_ok) {
        return -1;
    }

    const auto renderer_name = SDL_GetRendererName(renderer);
    if (!renderer_name) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to get renderer name: {}", error);
    }
    if (!init_ok) {
        return -1;
    }
    SPDLOG_INFO("SDL renderer name: {}", renderer_name);

    for (bool exit = false; exit == false; ) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                SPDLOG_INFO("event quit");
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

        if (!SetRenderDrawColor(renderer, COLOR_RED)) {
            SPDLOG_ERROR("failed to set renderer drawing color");
        }

        // TODO: replace with actual onscreen objects
        const auto rect = SDL_FRect { 50, 50, 100, 30 };
        if (!SDL_RenderFillRect(renderer, &rect)) {
            SPDLOG_ERROR("rendering failed");
        }

        if (!SDL_RenderPresent(renderer)) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("render present failed: {}", error);
        }
    }

    SPDLOG_INFO("destroy renderer");
    SDL_DestroyRenderer(renderer);

    SPDLOG_INFO("destroy window");
    SDL_DestroyWindow(window);

    SPDLOG_INFO("SDL library quit");
    SDL_Quit();

    return 0;
}