cmake_minimum_required (VERSION 2.8)

project (seamless)

set (SOURCES
  main.c
  pixel_access.c
  array_image.c
  energy_functions.c
)

set_source_files_properties (main.c pixel_access.c array_image.c energy_functions.c
  PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic-errors -std=c99 -O3"
  #PROPERTIES COMPILE_FLAGS "-Wall -Werror -pedantic-errors -std=c99"
)

include (FindSDL)
include (FindSDL_image)

if (${APPLE})
  set (SOURCES
    ${SOURCES}
    SDLMain.m
  )
endif (${APPLE})

include_directories (
  ${SDL_INCLUDE_DIR}
  ${SDLIMAGE_INCLUDE_DIR}
)

add_executable (seamless ${SOURCES})

target_link_libraries (seamless
  ${SDL_LIBRARY}
  ${SDLIMAGE_LIBRARY}
)
