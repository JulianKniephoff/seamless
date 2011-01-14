#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "util.h"
#include "pixel_access.h"

extern Uint8 gradient_magnitude (SDL_Surface * image, int x, int y);

extern Uint8 steepest_neighbor (SDL_Surface * image, int x, int y);

void
quit (void)
{
  IMG_Quit ();
  SDL_Quit ();
}

void
energize (SDL_Surface * image, SDL_Surface *energy,
	  Uint8 (*energy_function) (SDL_Surface *, int, int))
{
  /* Process image */
  if (SDL_MUSTLOCK (image))
    {
      SDL_LockSurface (image);
    }
  if (SDL_MUSTLOCK (energy))
    {
      SDL_LockSurface (energy);
    }
  Uint8 e;
  for (int x = 0; x < image->w; ++x)
    {
      for (int y = 0; y < image->h; ++y)
	{
	  e = energy_function (image, x, y);
	  put_pixel (energy, x, y, SDL_MapRGB (energy->format, e, e, e));
	}
    }
  if (SDL_MUSTLOCK (energy))
    {
      SDL_UnlockSurface (energy);
    }
  if (SDL_MUSTLOCK (image))
    {
      SDL_UnlockSurface (image);
    }
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
      fprintf (stderr, "Could not initialize SDL: %s\n",
	       SDL_GetError ());
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
      fprintf (stderr, "Could not set video mode: %s\n",
	       SDL_GetError ());
      return (EXIT_FAILURE);
    }
  SDL_Surface *energy =
    SDL_CreateRGBSurface (image->flags | SDL_SWSURFACE, image->w, image->h,
			  image->format->BitsPerPixel, image->format->Rmask,
			  image->format->Gmask, image->format->Bmask,
			  image->format->Amask);
  if (!energy)
    {
      fprintf (stderr, "Could not create an energy buffer: %s\n",
	       SDL_GetError ());
      return (EXIT_FAILURE);
    }


  /* Main loop */
  int running = 1;
  Uint32 black = SDL_MapRGB (screen->format, 0, 0, 0);
  SDL_Event event;
  Uint8 (*energy_function) (SDL_Surface *, int, int);
  energy_function = steepest_neighbor;
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
                    if (energy_function == steepest_neighbor)
                      {
                        energy_function = gradient_magnitude;
                      }
                    else
                      {
                        energy_function = steepest_neighbor;
                      }

                    energize (image, energy, energy_function);
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
