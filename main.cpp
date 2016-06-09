#include <SDL2/SDL.h>
#include <iostream>
#include <stdlib.h>

#define WIDTH 600
#define HEIGHT 800
int main() {
		bool running = true;
		SDL_Event e;
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

		while (running) 
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_WINDOWEVENT)
					if (e.window.event == SDL_WINDOWEVENT_CLOSE) // window was closed
						running = false;

				if (e.type == SDL_KEYDOWN)
					switch(e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
					}
			}
		}

		// Clean up
		SDL_DestroyWindow(window);
		SDL_Quit();
	return 0;
}
