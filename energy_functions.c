#include <math.h>

#include <SDL.h>

#include "util.h"
#include "pixel_access.h"

Uint8
gradient_magnitude (SDL_Surface * image, int x, int y)
{
  Uint8 r, g, b, l1, l2;
  float ddx, ddy;
  /* TODO Better handling of the borders... */
  /* d/dx */
  if (x == 0)
    {
      SDL_GetRGB (get_pixel (image, x, y), image->format, &r, &g, &b);
      l1 = max (max (r, g), b);
      SDL_GetRGB (get_pixel (image, x + 1, y), image->format, &r, &g, &b);
      l2 = max (max (r, g), b);
      ddx = l2 - l1;
    }
  else if (x == image->w - 1)
    {
      SDL_GetRGB (get_pixel (image, x - 1, y), image->format, &r, &g, &b);
      l1 = max (max (r, g), b);
      SDL_GetRGB (get_pixel (image, x, y), image->format, &r, &g, &b);
      l2 = max (max (r, g), b);
      ddx = l2 - l1;
    }
  else
    {
      SDL_GetRGB (get_pixel (image, x - 1, y), image->format, &r, &g, &b);
      l1 = max (max (r, g), b);
      SDL_GetRGB (get_pixel (image, x + 1, y), image->format, &r, &g, &b);
      l2 = max (max (r, g), b);
      ddx = (l2 - l1) / 2.0f;
    }
  /* d/dy */
  if (y == 0)
    {
      SDL_GetRGB (get_pixel (image, x, y), image->format, &r, &g, &b);
      l1 = max (max (r, g), b);
      SDL_GetRGB (get_pixel (image, x, y + 1), image->format, &r, &g, &b);
      l2 = max (max (r, g), b);
      ddy = l2 - l1;
    }
  else if (y == image->h - 1)
    {
      SDL_GetRGB (get_pixel (image, x, y - 1), image->format, &r, &g, &b);
      l1 = max (max (r, g), b);
      SDL_GetRGB (get_pixel (image, x, y), image->format, &r, &g, &b);
      l2 = max (max (r, g), b);
      ddy = l2 - l1;
    }
  else
    {
      SDL_GetRGB (get_pixel (image, x, y - 1), image->format, &r, &g, &b);
      l1 = max (max (r, g), b);
      SDL_GetRGB (get_pixel (image, x, y + 1), image->format, &r, &g, &b);
      l2 = max (max (r, g), b);
      ddy = (l2 - l1) / 2.0f;
    }

  return sqrt (ddx * ddx + ddy * ddy) * M_SQRT2;
}

Uint8
steepest_neighbor (SDL_Surface *image, int x, int y)
{
  Uint8 r, g, b, l, l1, l2, l3, l4, l5, l6, l7, l8;
  l1 = l2 = l3 = l4 = l5 = l6 = l7 = l8 = 0;

  SDL_GetRGB (get_pixel (image, x, y), image->format, &r, &g, &b);
  l = max (max (r, g), b);

  if (x > 0)
    {
      if (y > 0)
        {
          SDL_GetRGB (get_pixel (image, x - 1, y - 1), image->format, &r, &g, &b);
          l1 = abs(l - max (max (r, g), b));
        }
      SDL_GetRGB (get_pixel (image, x - 1, y), image->format, &r, &g, &b);
      l2 = abs(l - max (max (r, g), b));
      if (y < image->h - 1)
        {
          SDL_GetRGB (get_pixel (image, x - 1, y + 1), image->format, &r, &g, &b);
          l3 = abs(l - max (max (r, g), b));
        }
    }

  if (y > 0)
    {
      SDL_GetRGB (get_pixel (image, x, y - 1), image->format, &r, &g, &b);
      l4 = abs(l - max (max (r, g), b));
    }
  if (y < image->h - 1)
    {
      SDL_GetRGB (get_pixel (image, x, y + 1), image->format, &r, &g, &b);
      l5 = abs(l - max (max (r, g), b));
    }

  if (x < image->w - 1)
    {
      if (y > 0)
        {
          SDL_GetRGB (get_pixel (image, x + 1, y - 1), image->format, &r, &g, &b);
          l6 = abs(l - max (max (r, g), b));
        }
      SDL_GetRGB (get_pixel (image, x + 1, y), image->format, &r, &g, &b);
      l7 = abs(l - max (max (r, g), b));
      if (y < image->h - 1)
        {
          SDL_GetRGB (get_pixel (image, x + 1, y + 1), image->format, &r, &g, &b);
          l8 = abs(l - max (max (r, g), b));
        }
    }

  return max (max (max (max (max (max (max (l1, l2), l3), l4), l5), l6), l7), l8);
}
