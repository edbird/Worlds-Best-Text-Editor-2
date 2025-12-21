#ifndef SDLRENDERER_H
#define SDLRENDERER_H


#include <SDL2/SDL.h>


/*int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    const SDL_Color &color);*/


// non-const version
int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    SDL_Color color);


SDL_Color SDL_MakeColor(const Uint8 r, const Uint8 g, const Uint8 b);



#endif // SDLHELPER_H