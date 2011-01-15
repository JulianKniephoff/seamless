#include <math.h>

#include "util.h"
#include "array_image.h"

float
gradient_magnitude (Uint8 * image, int w, int h, int x, int y)
{
  Uint8 *pixel;
  Uint8 l1, l2;
  float ddx, ddy;
  /* TODO Better handling of the borders... */
  /* d/dx */
  if (x == 0)
    {
      pixel = array_p (image, w, x, y);
      l1 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      pixel += 3;
      l2 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      ddx = l2 - l1;
    }
  else if (x == w - 1)
    {
      pixel = array_p (image, w, x - 1, y);
      l1 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      pixel += 3;
      l2 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      ddx = l2 - l1;
    }
  else
    {
      pixel = array_p (image, w, x - 1, y);
      l1 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      pixel += 6;
      l2 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      ddx = (l2 - l1) / 2.0f;
    }
  /* d/dy */
  if (y == 0)
    {
      pixel = array_p (image, w, x, y);
      l1 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      pixel += 3 * w;
      l2 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      ddy = l2 - l1;
    }
  else if (y == h - 1)
    {
      pixel = array_p (image, w, x, y - 1);
      l1 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      pixel += 3 * w;
      l2 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      ddy = l2 - l1;
    }
  else
    {
      pixel = array_p (image, w, x, y - 1);
      l1 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      pixel += 6 * w;
      l2 = value_from_rgb (pixel[0], pixel[1], pixel[2]);
      ddy = (l2 - l1) / 2.0f;
    }

  return sqrt (ddx * ddx + ddy * ddy);
}

Uint8
steepest_neighbor (Uint8 * image, int w, int h, int x, int y)
{
  Uint8 l, l1, l2, l3, l4, l5, l6, l7, l8;
  l1 = l2 = l3 = l4 = l5 = l6 = l7 = l8 = 0;

  Uint8 *pixel = array_p (image, w, x, y);
  l = value_from_rgb (pixel[0], pixel[1], pixel[2]);

  if (x > 0)
    {
      if (y > 0)
	{
          pixel = array_p (image, w, x - 1, y - 1);
	  l1 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
	}
      pixel = array_p (image, w, x - 1, y);
      l2 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
      if (y < h - 1)
	{
          pixel = array_p (image, w, x - 1, y + 1);
	  l3 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
	}
    }

  if (y > 0)
    {
      pixel = array_p (image, w, x, y - 1);
      l1 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
    }
  pixel = array_p (image, w, x, y);
  l2 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
  if (y < h - 1)
    {
      pixel = array_p (image, w, x, y + 1);
      l3 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
    }

  if (x < w - 1)
    {
      if (y > 0)
        {
          pixel = array_p (image, w, x + 1, y - 1);
          l1 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
        }
      pixel = array_p (image, w, x + 1, y);
      l2 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
      if (y < h - 1)
        {
          pixel = array_p (image, w, x + 1, y + 1);
          l3 = abs (l - value_from_rgb (pixel[0], pixel[1], pixel[2]));
        }
    }

  return max (max (max (max (max (max (max (l1, l2), l3), l4), l5), l6), l7),
	      l8);
}
