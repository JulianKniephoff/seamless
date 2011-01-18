#include <stddef.h>
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
  int running = 1;
  Uint32 black = SDL_MapRGB (screen->format, 0, 0, 0);
  SDL_Event event;
  float *array = energize (image, gradient_magnitude);
  SDL_Surface *vis =
    energy_to_surface (array, image->flags, image->w, image->h,
		       image->format->BitsPerPixel, image->format->Rmask,
		       image->format->Gmask, image->format->Bmask,
		       image->format->Amask);
  while (running)
    {
      while (SDL_PollEvent (&event))
	{
	  switch (event.type)
	    {
	    case SDL_QUIT:
	      running = 0;
	      break;
            case SDL_KEYDOWN:
              switch (event.key.keysym.sym)
                {
                  case SDLK_q:
                    running = 0;
                    break;
                  default:
                    break;
                }
            }
	}

      SDL_FillRect (screen, NULL, black);

      SDL_BlitSurface (vis, NULL, screen, NULL);

      SDL_Flip (screen);
    }

  return (EXIT_SUCCESS);
}
