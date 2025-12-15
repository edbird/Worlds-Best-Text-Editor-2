#include "spdlog_util.hpp"

#include "application_configuration.hpp"
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

struct Document {

    void write_to_file(const char* filename) {
        std::ofstream ofile(filename);
        if (!ofile.is_open()) {
            throw std::runtime_error(
                std::format("failed to open file: {}", filename)
            );
        }

        for (const auto& line: lines) {
            ofile << line << '\n';
        }

        ofile.flush();
        ofile.close();

        SPDLOG_INFO("text data written to file {}", filename);
    }

    void read_from_file(const char* filename) {
        lines.clear();

        std::ifstream ifile(filename);
        if (!ifile.is_open()) {
            throw std::runtime_error(
                std::format("failed to open file: {}", filename)
            );
        }

        std::string line;
        while (std::getline(ifile, line)) {
            lines.push_back(line);
        }

        ifile.close();

        SPDLOG_INFO("text data read from file {}", filename);
    }

    std::vector<std::string> lines;
};

struct DocumentLayoutLine {
    std::size_t line_index;
    std::string_view text_span;
};

struct DocumentLayout {

    DocumentLayout(const uint64_t width_in_pixels)
        : width_in_pixels{width_in_pixels}
    {
    }

    void set_width(const uint64_t width_in_pixels) {
        this->width_in_pixels = width_in_pixels;
    }

    uint64_t width_in_pixels;
    std::vector<DocumentLayoutLine> lines;
};

std::size_t calculate_wrap_length(
    TTF_Text* ttf_text,
    const int width_in_pixels,
    const std::string& line,
    const std::size_t start
) {

    // Find the maximum number of characters which can be rendered within the
    // available space

    const auto lambda = [&line, &start, &ttf_text](const auto& index, const auto &width_in_pixels) -> bool {
        SPDLOG_INFO("lambda called: index={}, width_in_pixels={}", index, width_in_pixels);

        const auto substring = line.substr(start, index - start);

        if (!TTF_SetTextString(ttf_text, substring.c_str(), substring.size())) {
            throw std::runtime_error("TTF_SetTextString failure");
        }
        int w = 0;
        int h = 0;
        if (!TTF_GetTextSize(ttf_text, &w, &h)) {
            throw std::runtime_error("TTF_GetTextSize failure");
        }

        // TODO: check <= here or < ???
        return w <= static_cast<int64_t>(width_in_pixels);
    };

    const auto integer_range_it = std::views::iota(start, line.size());
    const auto end_it = std::lower_bound(
        integer_range_it.begin(),
        integer_range_it.end(),
        static_cast<std::size_t>(width_in_pixels),
        lambda
    );
    const auto end = *end_it;

    // At this point, the end index is known

    const auto substring_view = std::string_view(
        line.data() + start,
        line.data() + (end - start)
    );
    const auto find_pos = substring_view.find_last_of(' ');

    if (find_pos == std::string::npos) {
        // not found: the entire line is a single word or part of a single word
        SPDLOG_INFO("calculate_wrap_length: end of word not found, returning end={}, start={}, end-start={}", end, start, end-start);
        return end - start;
    }
    else {
        // found
        SPDLOG_INFO("calculate_wrap_length: end of word found at position {}, returning start={}, find_pos-start={}", find_pos, start, find_pos-start);
        return find_pos - start;
    }
}

DocumentLayout create_document_layout(
    const Document& text_file_container,
    TTF_Text* ttf_text,
    const int width_in_pixels
) {
    DocumentLayout document_layout(width_in_pixels);

    std::size_t line_index = 0;
    for (const auto& line: text_file_container.lines) {

        auto document_layout_line = DocumentLayoutLine();
        document_layout_line.line_index = line_index;

        std::size_t start = 0;
        while (start < line.size()) {
            const std::size_t wrap_length = calculate_wrap_length(
                ttf_text,
                width_in_pixels,
                line,
                start
            );

            document_layout_line.text_span = std::string_view(
                line.data() + start,
                wrap_length
            );

            document_layout.lines.push_back(document_layout_line);

            start += wrap_length;
        }

        ++ line_index;
    }

    return document_layout;
}

// TODO: move this, possibly simpler way to get the value
//const auto font_line_skip = TTF_GetFontLineSkip(TTF_GetTextFont(text_engine));

int main(int argc, char* argv[]) {

    // TODO: no longer required?
    bool init_ok = true;
    auto application_resources = ApplicationResources();

    SPDLOG_INFO("Worlds Best Text Editor startup");

    const auto maybe_application_configuration{initialize_application_configuration()};
    if (!maybe_application_configuration) {
        SPDLOG_ERROR("failed to initialize application configuration");
        init_ok = false;
        return -1;
    }
    const auto application_configuration{maybe_application_configuration.value()};

    const auto optional_font_path{application_configuration.getFontPath()};
    if (!optional_font_path) {
        SPDLOG_ERROR("failed to get font path from application configuration");
        init_ok = false;
        return -1;
    }
    const auto font_path{optional_font_path.value()};

    const auto optional_font_size{application_configuration.getFontSize()};
    if (!optional_font_size) {
        SPDLOG_ERROR("failed to get font size from application configuration");
        init_ok = false;
        return -1;
    }
    const auto font_size{optional_font_size.value()};

    // not sure if needed?
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");

    query_sdl_version();
    query_sdl_performance_counter_frequency();

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

    if (!initialize_text_engine(application_resources, renderer)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }
    const auto text_engine = application_resources.text_engine_list.front();

    if (!initialize_ttf_font(application_resources, font_path.c_str(), font_size)) {
        init_ok = false;
        cleanup(application_resources);
        return -1;
    }
    const auto ttf_font = application_resources.ttf_font_list.front();

    #if 0
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
    #endif

    #if 0
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
    #endif

    #if 0
    SDL_SetTextureScaleMode(text_texture, SDL_SCALEMODE_NEAREST);

    SDL_DestroySurface(text_surface);
    #endif

    const auto text1_string = "Hello world";
    const auto text1_length = strlen(text1_string);
    TTF_Text* text1 = TTF_CreateText(text_engine, ttf_font, text1_string, text1_length);

    Document text_file_container;
    text_file_container.read_from_file("example_textfile.txt");

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

        if (!SetRenderDrawColor(renderer, COLOR_RED)) {
            SPDLOG_ERROR("failed to set renderer drawing color");
        }

        // TODO: replace with actual onscreen objects
        const auto rect = SDL_FRect { 50, 50, 100, 30 };
        if (!SDL_RenderFillRect(renderer, &rect)) {
            SPDLOG_ERROR("rendering failed");
        }

        if (!SetRenderDrawColor(renderer, COLOR_MAGENTA)) {
            SPDLOG_ERROR("failed to set renderer drawing color");
        }

        TTF_DrawRendererText(text1, 200, 200);

        #if 0
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
        #endif



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
    }

    #if 0
    SPDLOG_INFO("destroy texture");
    SDL_DestroyTexture(text_texture);
    #endif

    SPDLOG_INFO("destroy text");
    TTF_DestroyText(text1);

    cleanup(application_resources);

    return 0;
}