#ifndef WINDOW_H
#define WINDOW_H

#include "SDL2/SDL.h"
#include <stdbool.h>

struct C3D_Window {
	SDL_Window *window;
	SDL_GLContext glContext;
	int width;
	int height;
};

typedef struct C3D_Window C3D_Window;

C3D_Window *window_create(int width, int height, const char *title);
C3D_Window *window_create_fullsize(const char *title);
void window_grab_cursor(C3D_Window *window, bool grab);
void window_fullscreen(C3D_Window *window, bool flag);

#endif
