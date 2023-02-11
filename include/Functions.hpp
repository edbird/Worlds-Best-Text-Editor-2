#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL.h>

void print_char_texture(SDL_Renderer *const renderer,
                        SDL_Texture* const texture,
                        const SDL_Rect& src_rect,
                        SDL_Rect& dst_rect);

#endif // FUNCTIONS_H
