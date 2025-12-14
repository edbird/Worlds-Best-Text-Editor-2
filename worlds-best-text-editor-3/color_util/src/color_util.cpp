#include "color_util.hpp"

#include <SDL3/SDL.h>



bool SetRenderDrawColor(SDL_Renderer *renderer, const SDL_Color &color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

SDL_Color Color(const Uint8 r, const Uint8 g, const Uint8 b) {
    const auto color = SDL_Color {r, g, b, 255};
    return color;
}