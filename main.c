#include <assert.h>

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "util.h"
#include "array_image.h"


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
  // fprintf (stderr, "column: %d\n", column);

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

      // printf ("%d %d %d %d\n", source1.x, source1.y, source1.w, source1.h);
      // printf ("%d %d %d %d\n", source2.x, source2.y, source2.w, source2.h);
      // printf ("%d %d %d %d\n", dest2.x, dest2.y, dest2.w, dest2.h);
      // printf ("%d\n", *pred);
      // printf ("---\n");

      // fprintf (stderr, "Source1: \n");
      // fprintf (stderr, "x: %d y: %d w: %d h: %d\n", source1.x, source1.y, source1.w, source1.h);
      SDL_BlitSurface (original, &source1, result, &source1);
      //fprintf (stderr, "x: %d y: %d w: %d h: %d\n", source1.x, source1.y, source1.w, source1.h);
      //fprintf (stderr, "Source2: \n");
      //fprintf (stderr, "x: %d y: %d w: %d h: %d\n", source2.x, source2.y, source2.w, source2.h);
      SDL_BlitSurface (original, &source2, result, &dest2);
      //fprintf (stderr, "x: %d y: %d w: %d h: %d\n", source2.x, source2.y, source2.w, source2.h);

      //fprintf (stderr, "source1.x: %d source1.w: %d source2.x: %d\n", source1.x, source1.w, source2.x);
      //fprintf (stderr, "seam dist: %d y: %d pred: %d pred dist: %d\n", source1.x + source1.w - source2.x, y, prev_pred, (int) (pred - preds));
      assert (source1.x + source1.w - source2.x == -1);
      //fprintf (stderr, "size diff: %d\n", original->w - source1.w - source2.w);
      assert (original->w - source1.w - source2.w == 1);
      assert (pred >= preds || y == 0);

      if (y != 0)
        {
          source1.w += *pred;
          source2.x += *pred;
          //fprintf (stderr, "w ist %d und wird verringert um %d\n", source2.w, *pred);
          source2.w -= *pred;
          dest2.w -= *pred;
          dest2.x = source2.x - 1;
          prev_pred = *pred;
          pred += *pred - original->w;
        }

      //fprintf (stderr, "---\n");
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
                    for (int i = 0; i < 50; ++i)
                      {
                        array = energize (image, steepest_neighbor);
                        pred = dynamic_program (array, image->w, image->h);

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
