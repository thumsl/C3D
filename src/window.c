#include "../include/window.h"
#include "../include/engine.h"
#include <GL/glew.h>

int createWindow(int width, int height, const char* title) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, SDL_GetError());
		return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// TODO: Create separate function for handling mouse grab?

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

	if (window == NULL) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	SDL_SetWindowFullscreen(window, FULLSCREEN);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	// TODO: destroy context on fail / on program ending
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return 1;
}

void grabCursor(bool grab) {
	if (grab) {
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetWindowGrab(window, SDL_TRUE);
	}
	else {
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetWindowGrab(window, SDL_FALSE);
	}
}