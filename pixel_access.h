#ifndef SEAMLESS_PIXEL_ACCESS_H
#define SEAMLESS_PIXEL_ACCESS_H

#include <SDL.h>

Uint32
get_pixel (SDL_Surface * surface, int x, int y);
void
put_pixel (SDL_Surface * surface, int x, int y, Uint32 pixel);

#endif
