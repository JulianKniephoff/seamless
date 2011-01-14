seamless: main.c energy_functions.c
	gcc -Wall -Werror -pedantic-errors -std=c99 `sdl-config --cflags --libs` -lSDL_image energy_functions.c pixel_access.c main.c -o seamless -O3
