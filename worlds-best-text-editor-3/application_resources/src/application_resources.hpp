#ifndef APPLICATION_RESOURCES_HPP
#define APPLICATION_RESOURCES_HPP

#include "spdlog_util.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_version.h>
#include <SDL3_ttf/SDL_ttf.h>

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

// TODO: move into a struct
extern const char* app_name;
extern const char *app_version;
extern const char *app_identifier;
extern const char *app_creator;
extern const char *app_copyright;
extern const char *app_url;
extern const char *app_type;

bool init_application_metadata();

void query_sdl_version();

void query_sdl_performance_counter_frequency();

/** Query video drivers
 */
void query_video_drivers();

/** Query render drivers
 */
void query_render_drivers();

bool query_renderer_name(SDL_Renderer* const renderer);

void cleanup(ApplicationResources& application_resources);

bool initialize_sdl(ApplicationResources &application_resources);

bool initialize_sdl_ttf(ApplicationResources &application_resources);

bool initialize_window(ApplicationResources &application_resources);

bool initialize_renderer(ApplicationResources &application_resources, SDL_Window* const window);

bool initialize_ttf_font(
    ApplicationResources &application_resources,
    const char* font_path
);

#endif