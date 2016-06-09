#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdio.h>

#define WIDTH 600
#define HEIGHT 800

int main() {
		int running = 1;
		SDL_Event e;
		SDL_Window *window;                    // Declare a pointer
		SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2 // SDL_GetError()?
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

		// A OpenGL context stores information about the state of the current
		// OpenGL instace, stuff like drawing triangles vs lines, clear color, etc.
		SDL_GLContext context = SDL_GL_CreateContext(window);

		if (context == NULL)
		{
			SDL_GetError();
			return 1;
		}

//		glewInit();
		printf("GL version: %s\n", glGetString(GL_VERSION));
		glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

		while (running) 
		{
			// input handling
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_WINDOWEVENT)
					if (e.window.event == SDL_WINDOWEVENT_CLOSE) // window was closed
						running = 0;

				if (e.type == SDL_KEYDOWN)
					switch(e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = 0;
							break;
					}
			}

			// rendering
			glClear(GL_COLOR_BUFFER_BIT);
			SDL_GL_SwapWindow(window);
		}

		// Clean up
		SDL_DestroyWindow(window);
		SDL_Quit();
	return 0;
}
