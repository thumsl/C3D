#include "window.h"

int createWindow(int width, int height, const char* title) {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, SDL_GetError());
		return 0;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

	if (window == NULL) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0; 
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	//TODO: destroy context on fail / on program ending
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return 1;
}
