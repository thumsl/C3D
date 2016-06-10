#include "engine.h"
#include <math.h>

int main(int argc, char* argv[]) {
	if (!createWindow(WIDTH, HEIGHT, "3D Game Engine"))
		return 1;

        if (!initOpenGL())
                return 1;

    GLuint program;
	if (!compileAndAttachShaders("shader.vert", "shader.frag", &program))
		return 1;

	GLfloat vertices[] = {
		-100.0f, -100.0f, -1, 1.0f, 0.0f, 0.0f,
		-50.0f,  -50.0f,  -1, 1.0f, 1.0f, 0.0f,
		-50.0f,  -100.0f, -1, 1.0f, 1.0f, 0.0f,
		 0,      -100.0f, -1, 1.0f, 1.0f, 1.0f
	};

	GLuint indices[] = {
        	2, 1, 3
	};

	GLuint indices2[] = {
        	0, 1, 2
	};

	mesh *quadrado = initMesh(vertices, indices, 4, 3);
	mesh *triangulo = initMesh(vertices, indices2, 4, 3);

	SDL_Event e;
	int running = 1;

	mat4x4 scale, translate, rotate, projection;

	GLuint scaleLocation = glGetUniformLocation(program, "scale");
	GLuint translateLocation = glGetUniformLocation(program, "translate");
	GLuint rotateLocation = glGetUniformLocation(program, "rotate");
	GLuint projectionLocation = glGetUniformLocation(program, "projection");

	mat4x4_gen_orthographic_projection(projection, WIDTH, HEIGHT, 100000.0f, 0.1f);
	glUniformMatrix4fv(projectionLocation, 1, 0, (GLfloat*)projection);

	float xFactor = 1.0f, yFactor = 1.0f, zFactor = 1.0f, factor = 0.1f, xFactor_t = 0.0f, yFactor_t = 0.0f, angle = 0.0f;
	short rot_x = 0, rot_y = 1, rot_z = 0;

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
					case SDLK_LEFT:
						xFactor -= factor;
						break;
					case SDLK_RIGHT:
						xFactor += factor;
						break;
					case SDLK_UP:
						yFactor += factor;
						break;
					case SDLK_DOWN:
						yFactor -= factor;
						break;
					case SDLK_a:
						xFactor_t -= factor;
						break;
					case SDLK_d:
						xFactor_t += factor;
						break;
					case SDLK_w:
						yFactor_t += factor;
						break;
					case SDLK_s:
						yFactor_t -= factor;
						break;
					case SDLK_j:
						angle -= 10;
						break;
					case SDLK_k:
						angle += 10;
						break;
				}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		mat4x4_gen_scale(scale, xFactor, yFactor, zFactor);
		glUniformMatrix4fv(scaleLocation, 1, 0, (GLfloat*)scale);

		mat4x4_gen_translate(translate, xFactor_t, yFactor_t, 0.0f);
		glUniformMatrix4fv(translateLocation, 1, 0, (GLfloat*)translate);
		
		mat4x4_gen_rotate(rotate, rot_x, rot_y, rot_z, angle);
		glUniformMatrix4fv(rotateLocation, 1, 0, (GLfloat*)rotate);

		draw(triangulo);
		draw(quadrado);

		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}

	return 0;
}
