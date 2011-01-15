#ifndef SEAMLESS_ARRAY_IMAGE_H
#define SEAMLESS_ARRAY_IMAGE_H

#include <SDL.h>


#define array_p(array, w, x, y) ((array) + 3 * ((w) * (y) + (x)))
#define array_r(array, w, x, y) ((array)[3 * ((w) * (y) + (x)) + 0])
#define array_g(array, w, x, y) ((array)[3 * ((w) * (y) + (x)) + 1])
#define array_b(array, w, x, y) ((array)[3 * ((w) * (y) + (x)) + 2])

#define make_array(width, height) (malloc (sizeof (Uint8 *) * 3 * width * height))

Uint8 *surface_to_array (SDL_Surface * surface);

SDL_Surface *array_to_surface (Uint8 * array, Uint32 flags, int width,
			       int height, int bitsPerPixel, Uint32 Rmask,
			       Uint32 Gmask, Uint32 Bmask, Uint32 Amask);


#endif
