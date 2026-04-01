#include "../include/window.h"
#include "../include/c3d.h"
#include <GL/glew.h>

void sdl_init_window()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(1);
	}
}

SDL_Window *window_init(int width, int height, const char *title)
{
	SDL_Window *window;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_WarpMouseInWindow(window, width / 2, height / 2);

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

C3D_Window *_c3d_window_new(SDL_Window *sdl_window)
{
	C3D_Window *c3d_window = malloc(sizeof(C3D_Window));
	c3d_window->window = sdl_window;
	c3d_window->glContext = SDL_GL_CreateContext(sdl_window);
	SDL_GetWindowSize(sdl_window, &c3d_window->width, &c3d_window->height);

	return c3d_window;
}

C3D_Window *window_create(int width, int height, const char *title)
{
	sdl_init_window();
	SDL_Window *sdl_window = window_init(width, height, title);

	if (sdl_window == NULL) {
		return NULL;
	}

	return _c3d_window_new(sdl_window);
}

C3D_Window *window_create_fullsize(const char *title)
{
	sdl_init_window();

	SDL_DisplayMode display_mode;

	if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0) {
		printf("SDL_GetDesktopDisplayMode Error: %s\n", SDL_GetError());
		SDL_Quit();
	}

	return window_create(display_mode.w, display_mode.h, title);
}

void window_grab_cursor(C3D_Window *window, bool grab)
{
	if (grab) {
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetWindowGrab(window->window, SDL_TRUE);
	} else {
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetWindowGrab(window->window, SDL_FALSE);
	}
}

void window_fullscreen(C3D_Window *window, bool flag)
{
	if (flag)
		SDL_SetWindowFullscreen(window->window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(window->window, 0);
}
