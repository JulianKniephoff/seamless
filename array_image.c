#include "array_image.h"

#include "pixel_access.h"


Uint8 *
surface_to_array (SDL_Surface * surface)
{
  if (!surface)
    {
      return NULL;
    }

  if (SDL_MUSTLOCK (surface))
    {
      SDL_LockSurface (surface);
    }

  Uint8 *array = malloc (sizeof (Uint8) * surface->w * surface->h * 3);
  if (!array)
    {
      return NULL;
    }
  /* TODO Maybe optimize this. It could be done a little more low level with one loop.
   * The question is then: How to generalize it.
   */
  Uint8 *pixel = array;
  for (int y = 0; y < surface->h; ++y)
    {
      for (int x = 0; x < surface->w; ++x)
	{
	  SDL_GetRGB (get_pixel (surface, x, y), surface->format, pixel,
		      pixel + 1, pixel + 2);
	  pixel += 3;
	}
    }

  if (SDL_MUSTLOCK (surface))
    {
      SDL_UnlockSurface (surface);
    }

  return array;
}


SDL_Surface *
array_to_surface (Uint8 * array, Uint32 flags, int width, int height,
		  int bitsPerPixel, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask,
		  Uint32 Amask)
{
  if (!array)
    {
      return NULL;
    }

  SDL_Surface *surface =
    SDL_CreateRGBSurface (flags, width, height, bitsPerPixel, Rmask, Gmask,
			  Bmask, Amask);
  if (!surface)
    {
      return NULL;
    }
  if (SDL_MUSTLOCK (surface))
    {
      SDL_LockSurface (surface);
    }

  Uint8 *pixel = array;
  for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
	{
	  put_pixel (surface, x, y,
		     SDL_MapRGB (surface->format, pixel[0], pixel[1],
				 pixel[2]));
	  pixel += 3;
	}
    }

  if (SDL_MUSTLOCK (surface))
    {
      SDL_UnlockSurface (surface);
    }
  return surface;
}
