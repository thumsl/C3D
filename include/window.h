#ifndef WINDOW_H
#define WINDOW_H
#include "SDL2/SDL.h"
#include <stdbool.h>

SDL_Window *window;

int createWindow(int width, int height, const char* title);
void grabCursor(bool grab);

#endif
