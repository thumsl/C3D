#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>

#define WIDTH 600
#define HEIGHT 800
int main() {
		SDL_Window *window;                    // Declare a pointer
		SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

		window = SDL_CreateWindow(
		    "3D Engine",                  // window title
		    SDL_WINDOWPOS_UNDEFINED,           // initial x position
		    SDL_WINDOWPOS_UNDEFINED,           // initial y position
		    WIDTH,                               // width, in pixels
		    HEIGHT,                               // height, in pixels
		    SDL_WINDOW_OPENGL                  // flags - see below
		);

		// Check that the window was successfully created
		if (window == NULL) {
		    // In the case that the window could not be made...
		    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		    exit(EXIT_FAILURE);
		}

		// The window is open: could enter program loop here (see SDL_PollEvent())

		SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

		// Close and destroy the window
		SDL_DestroyWindow(window);

		// Clean up
		SDL_Quit();
	return 0;
}
