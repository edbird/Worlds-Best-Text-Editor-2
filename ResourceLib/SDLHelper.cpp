#include "SDLHelper.h"


/*int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    const SDL_Color &color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, color.a);
}*/

// non const version
int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    SDL_Color color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, color.a);
}


SDL_Color SDL_MakeColor(const Uint8 r, const Uint8 g, const Uint8 b)
{
    const SDL_Color color {r, g, b, 255};
    return color;
}