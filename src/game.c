#include "../include/engine.h"
#include <math.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	bool running = true, mouseGrab = true;
	unsigned timePassed = 0, frames = 0, pastTime, currentTime = 0;
	int x = -1, y = -1, deltax = 0, deltay = 0, i;
	float angle = 0.0f, verticalAngle = 0.0f, horizontalAngle = PI, factor = 0;
	SDL_Event e;

	/* Create Window */
	if (!createWindow(WIDTH, HEIGHT, "3D Game Engine")) {
		quit();
		return 1;
	}

	/* Initialize OpenGL */
	if (!initOpenGL()) {
		quit();
		return 1;
	}

	/* Attach and compile shaders */
    shader S;
	if (!compileAndAttachShaders(&S, "src/glsl/shader.vert", "src/glsl/shader.frag")) {
		quit();
		return 1;
	}

	/* Get shader uniform locations, define light parameters */
	initShader(&S);
	vec4 pastDirection;
	vec3 lightColor = {0.9f, 0.9f, 0.7f}, center, nextPosition, pastPosition; 
	ambientLight ambient; float intensity = 0.5f;

	initAmbientLight(&ambient, lightColor, intensity);
	setAmbientLight(&ambient, &S);

	vec3 lightCol = {1.0f, 0.0f, 0.0f};
	vec3 lightPosition = {0.0f, 2.0f, 0.0f};
	float att = 0.5f; pointLight point;
	initPointLight(&point, lightCol, lightPosition, att);
	setPointLight(&point, &S);

	GLuint loc = glGetUniformLocation(S.program, "intensity");
	glUniform1f(loc, 3.0f);

	/* Set the projection matrix */
	mat4x4 model_view_projection, projection, view;
	mat4x4_perspective(projection, FOV, (float)WIDTH/(float)HEIGHT, 0.001f, 1000.f);

	/* Set the view matrix (camera) */
	// TODO: function to initialize camera with default values
	camera* C = initCamera();

	vec3_mul_cross(C->up, C->right, C->direction);
	vec3_add(center, C->eye, C->direction);
	mat4x4_look_at(view, C->eye, center, C->up);

	/* Initialize all meshes */
	short meshCount = 3; mesh* list[meshCount];
	list[0] = initOBJMesh("res/obj/jax.obj", "res/textures/jax.tga");
	list[1] = initOBJMesh("res/obj/raptor.obj", "res/textures/raptor.png");
	list[2] = initOBJMesh("res/obj/R2-D2.obj", "res/textures/R2-D2.tga");
	mesh_translate(list[0], -1, 0, 0);
	mesh_translate(list[2], 1, 0, 0);
	mesh* crosshair = initOBJMesh("res/obj/crosshair.obj", "res/textures/test.png");

	/* Define the player */
	player* P = initPlayer(C->eye);

	// TODO: WEAPON //
	
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
					running = false;
			if (e.type == SDL_KEYDOWN)
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						mouseGrab = !mouseGrab;
						grabCursor(mouseGrab);
						break;
					case SDLK_q:
						running = false;
						break;
					case SDLK_a:
						P->movement.left = 1;
						break;
					case SDLK_d:
						P->movement.right = 1;
						break;
					case SDLK_w:
						P->movement.forward = 1;
						break;
					case SDLK_s:
						P->movement.backward = 1;
						break;
					case SDLK_LSHIFT:
						printf("Running\n");
						P->movement.run = 1;
						break;
				}
			if (e.type == SDL_KEYUP)
				switch(e.key.keysym.sym) {
					case SDLK_a:
						P->movement.left = 0;
						break;
					case SDLK_d:
						P->movement.right = 0;
						break;
					case SDLK_w:
						P->movement.forward = 0;
						break;
					case SDLK_s:
						P->movement.backward = 0;
						break;
					case SDLK_LSHIFT:
						P->movement.run = 0;
						break;
				}
		}

	    SDL_GetMouseState(&x, &y);

	    mesh_rotate_from_ident(list[0], 0.0f, factor, 0.0f);
	    mesh_rotate_from_ident(list[1], 0.0f, factor, 0.0f);
	    mesh_rotate_from_ident(list[2], 0.0f, factor, 0.0f);
	    factor += 0.0005 * (currentTime - pastTime);
		
		/* Movement */
		vec3_copy(pastPosition, C->eye);
		vec3_copy(nextPosition, C->eye);
		camera_fps_move_simulate(nextPosition, C, P->movement, currentTime - pastTime);
		updateHitbox(P, nextPosition);
		if (aabb_collision(P->hitbox, list[0]->hitbox)) {
			DEBUG_PRINT(("Collision!\n"));
			vec3_copy(C->eye, pastPosition);
			updateHitbox(P, C->eye);
		}
		else {
			camera_fps_move(C, P->movement, currentTime - pastTime);
			updateHitbox(P, C->eye);
		}

		/* FPS camera control */
	    deltax = x - WIDTH/2;
	    deltay = y - HEIGHT/2;

	    if ((deltax != 0 || deltay != 0) && mouseGrab) {
			horizontalAngle += (float)(WIDTH/2 - x) * SENSITIVITY;
			verticalAngle += (float)(HEIGHT/2 - y) * SENSITIVITY;

			if (verticalAngle > 1.0f)
				verticalAngle = 1.0f;
			else if (verticalAngle < -1.0f)
				verticalAngle = -1.0f;

			camera_fps_mouse_look(C, horizontalAngle, verticalAngle);
			SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
	    }

		vec3_add(center, C->eye, C->direction);
		mat4x4_look_at(view, C->eye, center, C->up);		

		/* Rendering */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (i = 0; i < meshCount; i++) {
			mesh_update_model_matrix(list[i]);
			mat4x4_mul(model_view_projection, view, list[i]->transform.model);
			mat4x4_mul(model_view_projection, projection, model_view_projection);

			glUniformMatrix4fv(S.location.MVP, 1, 0, (GLfloat*)model_view_projection);

			draw(list[i]);
		}

		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}

	quit();

	return 0;
}
