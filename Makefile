OBJS = main.o array_image.o energy_functions.o pixel_access.o

CFLAGS = -c -Wall -Werror -pedantic-errors -std=c99 -O3
ifeq ($(DEBUG),1)
  CFLAGS += -g
endif

CFLAGS += `sdl-config --cflags`
LIBS = `sdl-config --libs` -lSDL_image

seamless: $(OBJS)
	gcc $(LIBS) $(OBJS) -o seamless

main.o: main.c array_image.h util.h
	gcc $(CFLAGS) main.c

array_image.o: array_image.c array_image.h pixel_access.h
	gcc $(CFLAGS) array_image.c

energy_functions.o: energy_functions.c util.h array_image.h
	gcc $(CFLAGS) energy_functions.c

pixel_access.o: pixel_access.c pixel_access.h
	gcc $(CFLAGS) pixel_access.c

clean:
	rm -f $(OBJS)
	rm seamless
