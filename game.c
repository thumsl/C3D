#include "engine.h"
#include <math.h>

int main(int argc, char* argv[]) {
	if (!createWindow(800, 600, "Game Engine"))
		return 1;

	if (!initOpenGL())
		return 1;

	if (!compileAndAttachShaders("shader.vert", "shader.frag"))
		return 1;

	SDL_Event e;
	int running = 1;

	GLfloat vertices[] = {
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};

    GLuint indices[] = {
        2, 1, 3
    };

    GLuint indices2[] = {
        0, 1, 2
    };

    mesh *quadrado = initMesh(vertices, indices, 4, 3);
	mesh *triangulo = initMesh(vertices, indices2, 4, 3);
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT)
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					running = 0;
			if (e.type == SDL_KEYDOWN)
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						running = 0;
						break;
					case SDLK_q:
						running = 0;
						break;
				}
		}

        glClear(GL_COLOR_BUFFER_BIT);

        draw(quadrado);
        draw(triangulo);

		SDL_GL_SwapWindow(window);

		SDL_Delay(1);
	}

	return 0;
}
