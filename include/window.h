#ifndef WINDOW_H
#define WINDOW_H
#include "SDL2/SDL.h"

SDL_Window *window;

int createWindow(int width, int height, const char* title);

#endif
