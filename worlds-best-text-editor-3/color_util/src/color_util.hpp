#ifndef COLOR_UTIL_HPP
#define COLOR_UTIL_HPP

// Must come first
#include "spdlog_util.hpp"

// External includes
#include <SDL3/SDL.h>

// Intenral includes

// C++ includes


// Library code

const auto COLOR_BLACK      = SDL_Color {   0,      0,      0,      0xff    };
const auto COLOR_RED        = SDL_Color {   255,    0,      0,      0xff    };
const auto COLOR_GREEN      = SDL_Color {   0,      255,    0,      0xff    };
const auto COLOR_BLUE       = SDL_Color {   0,      0,      255,    0xff    };
const auto COLOR_YELLOW     = SDL_Color {   255,    255,    0,      0xff    };
const auto COLOR_CYAN       = SDL_Color {   0,      255,    255,    0xff    };
const auto COLOR_MAGENTA    = SDL_Color {   255,    0,      255,    0xff    };
const auto COLOR_WHITE      = SDL_Color {   255,    255,    255,    0xff    };
const auto COLOR_GREY       = SDL_Color {   128,    128,    128,    0xff    };

const auto COLOR_BACKGROUND         = SDL_Color { 0xcc,  0xcc,   0xcc,      0xff    };
const auto COLOR_FOREGROUND         = SDL_Color {   0,      0,      0,      0xff    };
const auto COLOR_TEXT               = SDL_Color {   0,      0,      0,      0xff    };
const auto COLOR_LINE_BACKGROUND    = SDL_Color { 0xdd,  0xdd,   0xdd,      0xff    };

bool SetRenderDrawColor(SDL_Renderer *renderer, const SDL_Color &color);

// SDL_Color constructor function
SDL_Color Color(const Uint8 r, const Uint8 g, const Uint8 b);


#endif // COLOR_UTIL_HPP
