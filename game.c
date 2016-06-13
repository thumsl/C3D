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
    -1.0, -1.0,  1.0,    1.0, 0.0, 0.0,
     1.0, -1.0,  1.0,    0.0, 1.0, 0.0,
     1.0,  1.0,  1.0,    0.0, 0.0, 1.0,
    -1.0,  1.0,  1.0,    1.0, 1.0, 1.0,
    -1.0, -1.0, -1.0,    1.0, 0.0, 0.0,
     1.0, -1.0, -1.0,    0.0, 1.0, 0.0,
     1.0,  1.0, -1.0,    0.0, 0.0, 1.0,
    -1.0,  1.0, -1.0,    1.0, 1.0, 1.0
	};

	GLuint indices[] = {
		0, 1, 2, 2, 3, 0,
		1, 5, 6, 6, 2, 1,
		7, 6, 5, 5, 4, 7,
		4, 0, 3, 3, 7, 4,
		4, 5, 1, 1, 0, 4,
		3, 2, 6, 6, 7, 3
	};

	mesh *cubo = initMesh(vertices, indices, 8, 36);

	SDL_Event e;

	int running = 1, x = -1, y = -1, deltax = 0, deltay = 0;
	float xFactor = 1.0f, yFactor = 1.0f, zFactor = 1.0f, factor = 1.f, xFactor_t = 0.0f, yFactor_t = 0.0f, angle = 0.0f, verticalAngle = 0.0f, horizontalAngle = PI;
	
	GLuint MVP = glGetUniformLocation(program, "MVP");
	mat4x4 scale, translate, rotate, view, projection;

	mat4x4_gen_rotate(rotate, 1, 0, 0, 0);
	mat4x4_gen_scale(scale, xFactor, yFactor, zFactor);
	mat4x4_gen_translate(translate, xFactor_t, yFactor_t, 0.0f);
	mat4x4_perspective(projection, FOV, (float)WIDTH/(float)HEIGHT, 0.01f, 100.f);

	vec3 movement; movement[0] = 1.0f; movement[1] = 0.0f; movement[2] = 1.0f;
	vec3 eye; eye[0] = 0.0f; eye[1] = 2.0f; eye[2] = 4.0f;
	vec3 direction; direction[0] = cosf(verticalAngle) * sinf(horizontalAngle); direction[1] = sinf(verticalAngle); direction[2] = cosf(horizontalAngle) * cosf(verticalAngle);
	vec3 right; right[0] = sinf(horizontalAngle - PI/2.0f); right[1] = 0.0f; right[2] =  cosf(horizontalAngle - PI/2.0f);
	vec3 up; vec3_mul_cross(up, right, direction);
	
	vec3 center; vec3_add(center, eye, direction);
	mat4x4_look_at(view, eye, center, up);

	printf("position (Eye) --- %f %f %f\n", eye[0], eye[1], eye[2]);
	printf("direction --- %f %f %f\n", direction[0], direction[1], direction[2]);

	SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);

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
						mat4x4_gen_scale(scale, xFactor, yFactor, zFactor);
						break;
					case SDLK_RIGHT:
						xFactor += factor;
						mat4x4_gen_scale(scale, xFactor, yFactor, zFactor);
						break;
					case SDLK_UP:
						yFactor += factor;
						mat4x4_gen_scale(scale, xFactor, yFactor, zFactor);
						break;
					case SDLK_DOWN:
						yFactor -= factor;
						mat4x4_gen_scale(scale, xFactor, yFactor, zFactor);
						break;
					case SDLK_a:
						vec3_sub(eye, eye, right);
						vec3_add(center, eye, direction);
						break;
					case SDLK_d:
						vec3_add(eye, eye, right);
						vec3_add(center, eye, direction);
						break;
					case SDLK_w:
						vec3_add(eye, eye, direction);
						vec3_add(center, eye, direction);
						break;
					case SDLK_s:
						vec3_sub(eye, eye, direction);
						vec3_add(center, eye, direction);
						break;
					case SDLK_j:
						angle-=90;
						mat4x4_gen_rotate(rotate, 1, 0, 0, angle);
						break;
					case SDLK_k:
						angle += 90;
						mat4x4_gen_rotate(rotate, 1, 0, 0, angle);
						break;
				}
		}

	    SDL_GetMouseState(&x, &y);

	    deltax = x - WIDTH/2;
	    deltay = y - HEIGHT/2;
	    
	    if (deltax != 0 || deltay != 0) {
			horizontalAngle += (float)(WIDTH/2 - x) * SENSITIVITY;
			verticalAngle += (float)(HEIGHT/2 - y) * SENSITIVITY;
			printf("verticalAngle = %f\n", verticalAngle);
			if (verticalAngle > 1.5f)
				verticalAngle = 1.5f;
			else if (verticalAngle < -1.5f)
				verticalAngle = -1.5f;

			direction[0] = cosf(verticalAngle) * sinf(horizontalAngle);
			direction[1] = sinf(verticalAngle);
			direction[2] = cosf(verticalAngle) * cosf(horizontalAngle);

			right[0] = sinf(horizontalAngle - PI/2.0f);
			right[1] = 0;
			right[2] = cosf(horizontalAngle - PI/2.0f);

			vec3_mul_cross(up, right, direction);

			vec3_add(center, eye, direction);
			SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
	    }
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		mat4x4_look_at(view, eye, center, up);

		mat4x4 M1, M2, M3, M4;

		mat4x4_mul(M1, rotate, translate);
		mat4x4_mul(M2, scale, M1);
		mat4x4_mul(M3, view, M2);
		mat4x4_mul(M4, projection, M3);

		glUniformMatrix4fv(MVP, 1, 0, (GLfloat*)M4); // update this

		draw(cubo);

		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}

	return 0;
}
