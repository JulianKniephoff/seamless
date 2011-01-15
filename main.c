#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "util.h"
#include "array_image.h"


typedef float (*energy_function) (Uint8 * image, int width, int height, int x, int y);

extern float gradient_magnitude (Uint8 * image, int width, int height, int x, int y);
extern float steepest_neighbor (Uint8 * image, int width, int height, int x, int y);


/* TODO There should be more cleanup here... */
void
quit (void)
{
  IMG_Quit ();
  SDL_Quit ();
}

SDL_Surface *
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
  Uint8 *vis = make_array (image->w, image->h);

  float *res = result;
  Uint8 *pixel = vis;
  for (int y = 0; y < image->h; ++y)
    {
      for (int x = 0; x < image->w; ++x)
        {
          *res = f (array, image->w, image->h, x, y);
          pixel[0] = min (max (*res, 0), 255);
          pixel[1] = min (max (*res, 0), 255);
          pixel[2] = min (max (*res, 0), 255);

          ++res;
          pixel += 3;
        }
    }

  return array_to_surface (vis, image->flags, image->w, image->h,
			   image->format->BitsPerPixel, image->format->Rmask,
			   image->format->Gmask, image->format->Bmask,
			   image->format->Amask);
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
  energy_function f = steepest_neighbor;
  SDL_Surface *energy = energize (image, f);
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
		case SDLK_RETURN:
		  if (f == steepest_neighbor)
		    {
		      f = gradient_magnitude;
		    }
		  else
		    {
		      f = steepest_neighbor;
		    }

		  SDL_FreeSurface (energy);
		  energy = energize (image, f);
		  break;
		default:
		  break;
		}
	      break;
	    }
	}

      SDL_FillRect (screen, NULL, black);

      SDL_BlitSurface (energy, NULL, screen, NULL);

      SDL_Flip (screen);
    }

  return (EXIT_SUCCESS);
}
