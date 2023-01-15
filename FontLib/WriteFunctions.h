#ifndef WRITE_FUNCTIONS_H
#define WRITE_FUNCTIONS_H


// Internal includes
#include "FontManager.h"
////#include "Color.h"
// TODO: fix cmake problem
#include "../ColorLib/Color.h"

// External includes
#include <SDL2/SDL.h>

// C++ includes
#include <memory>


// TODO: might need to do some perfomance metrics
// and optimization on these functions since they
// are likely to be the most called GUI function
// and probably one potential performance critical
// function call

void write(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const Uint16 c,
    int &x, const int y,
    const bool advance = false);


void writeWithBackground(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const Uint16 c,
    int &x, const int y,
    const bool advance = false,
    const SDL_Color &backgroundColor = COLOR_WHITE);


void writeString(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const std::string &text,
    int &x, const int y,
    const bool advance = false);


void writeStringWithBackground(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const std::string &text,
    int &x, const int y,
    const bool advance = false,
    const SDL_Color &backgroundColor = COLOR_WHITE);


void writeStringWithTickTockBackground(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const std::string &text,
    int &x, const int y,
    const bool advance = false,
    const SDL_Color &backgroundColor1 = COLOR_WHITE,
    const SDL_Color &backgroundColor2 = COLOR_GREY);


void writeCustomSymbol(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    int &x, const int y,
    const bool advance = false);


void debugDrawCharsTexture(
    std::shared_ptr<SDL_Renderer> sdlRenderer,
    std::shared_ptr<FontTexture> fontTexture,
    const int x, const int y);


#endif // WRITE_FUNCTIONS_H