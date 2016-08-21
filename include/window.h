#ifndef WINDOW_H
#define WINDOW_H

#include "SDL2/SDL.h"
#include <stdbool.h>

SDL_Window* window_create(int width, int height, const char* title);
void window_grabCursor(SDL_Window* window, bool grab);
void window_fullscreen(SDL_Window *window, bool flag);

#endif
