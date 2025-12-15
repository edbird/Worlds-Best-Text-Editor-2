#include "spdlog_util.hpp"

#include "application_resources.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_version.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>


void query_sdl_version() {
    const auto sdl_version = SDL_VERSION;
    SPDLOG_INFO("SDL Version: {}", sdl_version);
}

void query_sdl_performance_counter_frequency() {
    const auto frequency = SDL_GetPerformanceFrequency();
    SPDLOG_INFO("SDL performance counter frequency: {}", frequency);
}

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

bool query_renderer_name(SDL_Renderer* const renderer) {

    const auto renderer_name = SDL_GetRendererName(renderer);
    if (!renderer_name) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to get renderer name: {}", error);
        return false;
    }

    SPDLOG_INFO("SDL renderer name: {}", renderer_name);

    return true;
}

// TODO: move into a struct
const char* app_name = "Worlds-Best-Text-Editor";
const char *app_version = "v0.1.0";
const char *app_identifier = "worldsbesttexteditor";
const char *app_creator = "Edward Birdsall";
const char *app_copyright = "All rights reserved";
const char *app_url = "https://www.linkedin.com/in/edward-b-a71830218";
const char *app_type = "application";

bool init_application_metadata() {

    if (!SDL_SetAppMetadata(app_name, app_version, app_identifier)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metadata: {}", error);
        return false;
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, app_creator)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (creator): {}", error);
        return false;
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, app_copyright)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (copyright): {}", error);
        return false;
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, app_url)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (url): {}", error);
        return false;
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, app_type)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to set app metdata (): {}", error);
        return false;
    }

    return true;
}

void cleanup(ApplicationResources& application_resources) {

    while (application_resources.ttf_font_list.size() > 0) {
        const auto ttf_font = application_resources.ttf_font_list.back();
        application_resources.ttf_font_list.pop_back();
        const auto index = application_resources.ttf_font_list.size(); // note: index correct, not -1 required

        SPDLOG_INFO("destroy fonts");
        TTF_CloseFont(ttf_font);
    }

    while (application_resources.renderer_list.size() > 0) {
        const auto renderer = application_resources.renderer_list.back();
        application_resources.renderer_list.pop_back();
        const auto index = application_resources.renderer_list.size(); // note: index correct, not -1 required

        SPDLOG_INFO("destroy renderer {}", index);
        SDL_DestroyRenderer(renderer);
    }

    while (application_resources.window_list.size() > 0) {
        const auto window = application_resources.window_list.back();
        application_resources.window_list.pop_back();
        const auto index = application_resources.window_list.size(); // note: index correct, not -1 required

        SPDLOG_INFO("destroy window {}", index);
        SDL_DestroyWindow(window);
    }

    if (application_resources.sdl_ttf_init) {
        SPDLOG_INFO("SDL TTF library quit");
        TTF_Quit();
    }

    if (application_resources.sdl_init) {
        SPDLOG_INFO("SDL library quit");
        SDL_Quit();
    }
}

bool initialize_sdl(ApplicationResources &application_resources) {

    application_resources.sdl_init = true;

    SPDLOG_INFO("SDL library initialize");
    const auto sdl_initflags = SDL_INIT_VIDEO;
    if (!SDL_Init(sdl_initflags)) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("SDL init failed: {}", error);
        return false;
    }
    return true;
}

bool initialize_sdl_ttf(ApplicationResources &application_resources) {

    application_resources.sdl_ttf_init = true;

    SPDLOG_INFO("SDL TTF library initialize");
    if (!TTF_Init()) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("SDL TTF init failed: {}", error);
        return false;
    }
    return true;
}

bool initialize_window(ApplicationResources &application_resources) {

    SPDLOG_INFO("SDL create window");
    const auto window_flags = SDL_WINDOW_RESIZABLE;
    const auto window = SDL_CreateWindow("Worlds Best Text Editor", 800, 600, window_flags);
    if (!window) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to create window: {}", error);
        return false;
    }

    application_resources.window_list.push_back(window);

    return true;
}

bool initialize_renderer(ApplicationResources &application_resources, SDL_Window* const window) {

    SPDLOG_INFO("SDL create renderer");
    SDL_Renderer *renderer = SDL_CreateRenderer(window, "gpu,opengl");
    if (!renderer) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to create renderer: {}", error);
        return false;
    }

    application_resources.renderer_list.push_back(renderer);

    return true;
}

bool initialize_text_engine(
    ApplicationResources &application_resources,
    SDL_Renderer* renderer
) {

    SPDLOG_INFO("SDL TTF create text engine");
    TTF_TextEngine* text_engine = TTF_CreateRendererTextEngine(renderer);
    if (!text_engine) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to create text engine: {}", error);
        return false;
    }

    application_resources.text_engine_list.push_back(text_engine);

    return true;
}

bool initialize_ttf_font(
    ApplicationResources &application_resources,
    const char* font_path,
    const unsigned int font_size
) {

    SPDLOG_INFO("SDL TTF create ttf font from path {}, {} size pt", font_path, font_size);
    const float ttf_font_ptsize = static_cast<float>(font_size);
    const auto ttf_font = TTF_OpenFont(font_path, ttf_font_ptsize);
    if (!ttf_font) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to open ttf font: {}", error);
        return false;
    }

    application_resources.ttf_font_list.push_back(ttf_font);

    return true;
}
