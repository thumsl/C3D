#include "../include/window.h"
#include "../include/c3d.h"
#include <GL/glew.h>

SDL_Window* window_create(int width, int height, const char* title) {
	SDL_Window* window;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return NULL;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_WarpMouseInWindow(window, width/2, height/2);

	if (window == NULL) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		SDL_Quit();
		return NULL;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	// TODO: destroy context on fail / on program ending
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return window;
}

void window_grabCursor(SDL_Window *window, bool grab) {
	if (grab) {
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetWindowGrab(window, SDL_TRUE);
	}
	else {
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetWindowGrab(window, SDL_FALSE);
	}
}

void window_fullscreen(SDL_Window *window, bool flag) {
	if (flag)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(window, 0);
}
