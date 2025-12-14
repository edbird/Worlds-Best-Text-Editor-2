#include "spdlog_util.hpp"

#include "color_util.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_version.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
// #include <SDL3/SDL_render.h>
// #include <SDL3/SDL_error.h>
// #include <SDL3/SDL_init.h>

#include <print>
#include <format>


void query_sdl_version() {
    const auto sdl_version = SDL_VERSION;
    SPDLOG_INFO("SDL Version: {}", sdl_version);
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

struct ApplicationResources {
    bool sdl_init;
    bool sdl_ttf_init;
    std::vector<SDL_Window*> window_list;
    std::vector<SDL_Renderer*> renderer_list;
    std::vector<TTF_Font*> ttf_font_list;

    ApplicationResources()
        : sdl_init{false}
        , sdl_ttf_init{false}
    {
    }
};


// struct InitializedSystems {
//     bool sdl;
//     bool sdl_ttf;
//     std::vector<SDL_Renderer*> sdl_renderer;
//     std::vector<SDL_Window*> sdl_window;
//     // TODO: add others, implement this so that shutdown destructs objects correctly
//     // TODO: add fonts
// };

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

bool initialize_ttf_font(ApplicationResources &application_resources) {

    const float ttf_font_ptsize = 10.0f;
    const auto ttf_font_file_path = "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf";
    const auto ttf_font = TTF_OpenFont(ttf_font_file_path, ttf_font_ptsize);
    if (!ttf_font) {
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to open ttf font: {}", error);
        return false;
    }

    application_resources.ttf_font_list.push_back(ttf_font);

    return true;
}

int main(int argc, char* argv[]) {

    // TODO: no longer required?
    bool init_ok = true;
    auto application_resources = ApplicationResources();

    SPDLOG_INFO("Worlds Best Text Editor startup");

    query_sdl_version();

    if (!init_application_metadata()) {
        init_ok = false;
        return -1;
    }

    query_video_drivers();
    query_render_drivers();

    if (!initialize_sdl(application_resources)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }

    if (!initialize_sdl_ttf(application_resources)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }

    if (!initialize_window(application_resources)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }
    const auto window = application_resources.window_list.front();

    if (!initialize_renderer(application_resources, window)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }
    const auto renderer = application_resources.renderer_list.front();

    if (!query_renderer_name(renderer)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }

    if (!initialize_ttf_font(application_resources)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }
    const auto ttf_font = application_resources.ttf_font_list.front();

    SDL_Surface *text_surface = TTF_RenderText_Solid(ttf_font, "hello world!", 0, COLOR_GREEN);
    if (!text_surface) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to render ttf font to surface: {}", error);
    }
    if (!init_ok) {
        // TODO: will not cleanup other subsystems correctly
        return -1;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture) {
        init_ok = false;
        const auto error = SDL_GetError();
        SPDLOG_ERROR("failed to convert surface to texture: {}", error);
    }
    if (!init_ok) {
        // TODO: will not cleanup other subsystems correctly
        return -1;
    }

    SDL_SetTextureScaleMode(text_texture, SDL_SCALEMODE_NEAREST);

    SDL_DestroySurface(text_surface);

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

        int w = 0;
        int h = 0;
        SDL_FRect dst;
        {
            const float scale = 4.0;
            SDL_SetRenderScale(renderer, scale, scale);
        }
        SDL_GetRenderOutputSize(renderer, &w, &h);
        SDL_GetTextureSize(text_texture, &dst.w, &dst.h);
        dst.x = 50;
        dst.y = 50;

        if (!SDL_RenderTexture(renderer, text_texture, nullptr, &dst)) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("failed to render texture: {}", error);
        }

        {
            const float scale = 1.0;
            SDL_SetRenderScale(renderer, scale, scale);
        }

        if (!SDL_RenderPresent(renderer)) {
            const auto error = SDL_GetError();
            SPDLOG_ERROR("render present failed: {}", error);
        }
    }

    SPDLOG_INFO("destroy texture");
    SDL_DestroyTexture(text_texture);

    cleanup(application_resources);

    return 0;
}