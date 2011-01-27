#include <assert.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array_image.h"
#include "util.h"


typedef float (*energy_function) (Uint8 * image, int width, int height, int x,
				  int y);

extern float gradient_magnitude (Uint8 * image, int width, int height, int x,
				 int y);
extern float steepest_neighbor (Uint8 * image, int width, int height, int x,
				int y);


/* TODO There should be more cleanup here... */
void
quit (void)
{
  IMG_Quit ();
  SDL_Quit ();
}

float *
energize (SDL_Surface * image, energy_function f)
{
  Uint8 *array = surface_to_array (image);

  if (!array)
    {
      return NULL;
    }
  float *result = malloc (sizeof (float) * image->w * image->h);
  if (!result)
    {
      free (array);
      return NULL;
    }

  float *res = result;
  for (int y = 0; y < image->h; ++y)
    {
      for (int x = 0; x < image->w; ++x)
	{
	  *res = f (array, image->w, image->h, x, y);

	  ++res;
	}
    }

  return result;
}

/*
 * TODO Think about the tradeoff between storing the predecessor index vs. horizontal offset:
 * index makes "backtracking" faster...
 */
char *
dynamic_program (float * array, int width, int height)
{
  char *pred = malloc (sizeof (char) * width * (height - 1));
  float min;
  char min_pos;
  float *preds = array;
  float *pixel = array + width;
  char *pixel_pred = pred;
  for (int y = 1; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
        {
          if (x > 0)
            {
              min = preds[-1];
              min_pos = -1;
            }
          else
            {
              min = preds[0];
              min_pos = 0;
            }

          if (preds[0] < min)
            {
              ++min_pos;
              min = preds[0];
            }

          if (x < width - 1)
            {
              if (preds[1] < min)
                {
                  ++min_pos;
                  min = preds[1];
                }
            }

          *pixel_pred = min_pos;
          ++pixel_pred;
          *pixel += min;
          ++pixel;

          ++preds;
        }
    }

  return pred;
}

unsigned int *
removal_indices (float *energy, int width, int height)
{
  unsigned int *result = malloc (sizeof (unsigned int) * width * height);
  memset (result, -1, sizeof (unsigned int) * width * height);
  int *pred = malloc (sizeof (int) * width * (height - 1));

  float min;
  int min_pos;
  float *pixel = energy + width;
  float *preds = energy;
  int *pixel_preds = pred;
  unsigned int *pred_indices = result;
  unsigned int *pixel_indices = result + width;
  int l = 0, m, r;
  for (unsigned int k = 0; k < width; ++k)
    {
      pred_indices = result;
      pixel_indices = result + width;
      pixel = energy + width;
      preds = energy;
      pixel_preds = pred;

      for (int y = 1; y < height; ++y)
        {
          m = 0;
          while (*(pred_indices++) < k)
            {
              ++preds;
              ++m;
            }
          while (*(pixel_indices++) < k)
            {
              ++pixel;
              ++pixel_preds;
              --m;
            }

          for (int x = 0; x < width - k; ++x)
            {
              if (x > 0)
                {
                  min = preds[l];
                  min_pos = l;
                }
              else
                {
                  min = *preds;
                  min_pos = m;
                }
              if (*preds < min)
                {
                  min = *preds;
                  min_pos = m;
                }
              r = m;
              if (x < width - k - 1)
                {
                  do
                    {
                      ++preds;
                      ++r;
                    } while (*(++pred_indices) < k);
                  if (*preds < min)
                    {
                      min = *preds;
                      min_pos = r;
                    }
                }
              
              fprintf (stderr, "%d %d %d\n", l, m, r);

              *pixel += min;
              *pixel_preds = min_pos;

              l = m;
              m = r;
              do
                {
                  ++pixel;
                  ++pixel_preds;
                  --l;
                  --m;
                } while (*(++pixel_indices) < k);
            }
          fprintf (stderr, "---\n");
        }

      // pixel = energy + 
      // min = 
      // for (int x = 0; x < width; ++x)
      //   {}
    }

  free (pred);

  return result;
}

int
min_pixel (float *array, int width, int height)
{
  int min_pos = 0;
  float *pixel = array + (height - 1) * width;
  float min = *pixel;
  for (int x = 0; x < width; ++x, ++pixel)
    {
      if (*pixel < min)
        {
          min = *pixel;
          min_pos = x;
        }
    }
  return min_pos;
}

SDL_Surface *
remove_seam (SDL_Surface * original, int column, char * preds)
{
  SDL_Surface * result = SDL_CreateRGBSurface (original->flags, original->w - 1, original->h, original->format->BitsPerPixel,
                                               original->format->Rmask, original->format->Gmask, original->format->Bmask,
                                               original->format->Amask);
  if (!result)
    {
      return NULL;
    }

  SDL_Rect source1, source2, dest2;
  source1.x = 0;
  source1.w = column;
  source1.h = source2.h = 1;
  source2.x = column + 1;
  source2.w = original->w - column - 1;
  char *pred = preds + (original->h - 2) * original->w + column;
  memcpy (&dest2, &source2, sizeof (SDL_Rect));
  dest2.x = source2.x - 1;
  char prev_pred;
  for (int y = original->h - 1; y >= 0; --y)
    {
      source1.h = source2.h = dest2.h = 1;
      source1.y = source2.y = dest2.y = y;

      SDL_BlitSurface (original, &source1, result, &source1);
      SDL_BlitSurface (original, &source2, result, &dest2);

      if (y != 0)
        {
          source1.w += *pred;
          source2.x += *pred;
          source2.w -= *pred;
          dest2.w -= *pred;
          dest2.x = source2.x - 1;
          prev_pred = *pred;
          pred += *pred - original->w;
        }
    }
  
  return result;
}

SDL_Surface *
energy_to_surface (float *array, Uint32 flags, int width,
		   int height, int bitsPerPixel, Uint32 Rmask,
		   Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
  Uint8 *result = malloc (sizeof (Uint8) * 3 * width * height), *pixel =
    result;
  
  float max = 0.0f;
  for (int i = 0; i < width * height; ++i)
    {
      if (array[i] > max)
        {
          max = array[i];
        }
    }
  float norm = 1.0f / max * 255;

  for (int i = 0; i < width * height; ++i)
    {
      pixel[0] = array[i] * norm;
      pixel[1] = array[i] * norm;
      pixel[2] = array[i] * norm;
      pixel += 3;
    }

  return array_to_surface (result, flags, width, height, bitsPerPixel, Rmask,
			   Gmask, Bmask, Amask);
}

int
main (int argc, char *argv[])
{
  /* Process command line arguments */
  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s <image path>\n", argv[0]);
      return (EXIT_FAILURE);
    }
  char *image_path = argv[1];

  /* Initialize subsystems */
  atexit (quit);
  if (SDL_Init (SDL_INIT_VIDEO))
    {
      fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError ());
      return (EXIT_FAILURE);
    }
  char title[FILENAME_MAX];
  sprintf (title, "seamless - %s", image_path);
  SDL_WM_SetCaption (title, title);
  if (IMG_Init (0))
    {
      fprintf (stderr, "Could not initialize SDL_image: %s\n",
	       IMG_GetError ());
      return (EXIT_FAILURE);
    }

  /* Prepare drawing */
  SDL_Surface *image = IMG_Load (image_path);
  if (!image)
    {
      fprintf (stderr, "Could not load image %s: %s\n", image_path,
	       IMG_GetError ());
      return (EXIT_FAILURE);
    }
  SDL_Surface *screen =
    SDL_SetVideoMode (image->w, image->h, image->format->BitsPerPixel,
		      SDL_SWSURFACE);
  if (!screen)
    {
      fprintf (stderr, "Could not set video mode: %s\n", SDL_GetError ());
      return (EXIT_FAILURE);
    }


  /* Main loop */
  bool running = true;
  Uint32 black = SDL_MapRGB (screen->format, 0, 0, 0);
  SDL_Event event;
  float *array;
  char *pred;
  SDL_Surface *original;

  while (running)
    {
      while (SDL_PollEvent (&event))
	{
	  switch (event.type)
	    {
	    case SDL_QUIT:
	      running = false;
	      break;
            case SDL_KEYDOWN:
              switch (event.key.keysym.sym)
                {
                  case SDLK_q:
                  case SDLK_ESCAPE:
                    running = 0;
                    break;
                  case SDLK_RETURN:
                    for (int i = 0; i < 1; ++i)
                      {
                        array = energize (image, steepest_neighbor);
                        pred = dynamic_program (array, image->w, image->h);
                        free (removal_indices (array, image->w, image->h));

                        original = image;
                        image = remove_seam (original,
                          min_pixel (array, original->w, original->h), pred);

                        SDL_FreeSurface (original);
                        free (array);
                        free (pred);
                      }
                    break;
                  default:
                    break;
                }
            }
	}

      SDL_FillRect (screen, NULL, black);

      SDL_BlitSurface (image, NULL, screen, NULL);

      SDL_Flip (screen);
    }

  return (EXIT_SUCCESS);
}
