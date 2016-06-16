#include "../include/engine.h"
#include <math.h>

int main(int argc, char* argv[]) {
	if (!createWindow(WIDTH, HEIGHT, "3D Game Engine")) {
		quit();
		return 1;
	}

	if (!initOpenGL()) {
		quit();
		return 1;
	}

    GLuint program;
	if (!compileAndAttachShaders("src/glsl/shader.vert", "src/glsl/shader.frag", &program)) {
		quit();
		return 1;
	}

	mesh* M = initOBJMesh(argv[1], argv[2]);

	SDL_Event e;

	unsigned short running = 1, mvup = 0, mvdown = 0, mvleft = 0, mvright = 0, timePassed = 0, frames = 0, pastTime, currentTime = 0;;
	int x = -1, y = -1, deltax = 0, deltay = 0;
	float xFactor = 1.0f, yFactor = 1.0f, zFactor = 1.0f, factor = 1.f, xFactor_t = 0.0f, yFactor_t = 0.0f, angle = 0.0f, verticalAngle = 0.0f, horizontalAngle = PI;
	
	GLuint MVP = glGetUniformLocation(program, "MVP");
	mat4x4 scale, translate, rotate, view, projection;

	mat4x4_gen_rotate(rotate, 1, 0, 0, 0);
	mat4x4_gen_scale(scale, xFactor, yFactor, zFactor);
	mat4x4_gen_translate(translate, xFactor_t, yFactor_t, 0.0f);
	mat4x4_perspective(projection, FOV, (float)WIDTH/(float)HEIGHT, 0.01f, 100.f);

	camera C;
	C.eye[0] = 0.0f; C.eye[1] = 2.0f; C.eye[2] = 4.0f;
	C.direction[0] = cosf(verticalAngle) * sinf(horizontalAngle); C.direction[1] = sinf(verticalAngle); C.direction[2] = cosf(horizontalAngle) * cosf(verticalAngle);
	C.right[0] = sinf(horizontalAngle - PI/2.0f); C.right[1] = 0.0f; C.right[2] =  cosf(horizontalAngle - PI/2.0f);
	vec3_mul_cross(C.up, C.right, C.direction);
	
	vec3 center; vec3_add(center, C.eye, C.direction);
	mat4x4_look_at(view, C.eye, center, C.up);

	SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);

	while (running) {
		pastTime = currentTime;
		currentTime = SDL_GetTicks();
		timePassed += (currentTime - pastTime);
		frames++;
		if (timePassed >= 1000) {
			timePassed = 0;
			printf("%d FPS\n", frames);
			frames = 0;
		}	

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
						mvleft = 1;
						break;
					case SDLK_d:
						mvright = 1;
						break;
					case SDLK_w:
						mvup = 1;
						break;
					case SDLK_s:
						mvdown = 1;
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
			if (e.type == SDL_KEYUP)
				switch(e.key.keysym.sym) {
					case SDLK_a:
						mvleft = 0;
						break;
					case SDLK_d:
						mvright = 0;
						break;
					case SDLK_w:
						mvup = 0;
						break;
					case SDLK_s:
						mvdown = 0;
						break;
				}
		}

	    SDL_GetMouseState(&x, &y);

   		camera_fps_move(&C, mvup, mvdown, mvleft, mvright, currentTime - pastTime);

	    deltax = x - WIDTH/2;
	    deltay = y - HEIGHT/2;

	    if (deltax != 0 || deltay != 0) {
			horizontalAngle += (float)(WIDTH/2 - x) * SENSITIVITY;
			verticalAngle += (float)(HEIGHT/2 - y) * SENSITIVITY;

			if (verticalAngle > 1.5f)
				verticalAngle = 1.5f;
			else if (verticalAngle < -1.5f)
				verticalAngle = -1.5f;

			camera_fps_mouse_look(&C, horizontalAngle, verticalAngle);

			SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
	    }
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vec3_add(center, C.eye, C.direction);
		mat4x4_look_at(view, C.eye, center, C.up);

		mat4x4 M1, M2, M3, M4;
		mat4x4_mul(M1, rotate, translate);
		mat4x4_mul(M2, scale, M1);
		mat4x4_mul(M3, view, M2);
		mat4x4_mul(M4, projection, M3);

		glUniformMatrix4fv(MVP, 1, 0, (GLfloat*)M4);

		draw(M);

		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}

	quit();

	return 0;
}
