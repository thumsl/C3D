#include "../include/engine.h"
#include <math.h>

int main(int argc, char* argv[]) {
	bool running = true, mouseGrab = true, drawBoundingBox = false;
	unsigned int timePassed = 0, frames = 0, pastTime, currentTime = 0, frameTime;
	int x = -1, y = -1, deltax = 0, deltay = 0, i;
	float angle = 0.0f, verticalAngle = 0.0f, horizontalAngle = PI, factor = 0;
	SDL_Event e;

	/* Create Window */
	SDL_Window* window = window_create(WIDTH, HEIGHT, "C3D Game Engine");
	if (window == NULL) {
		fprintf(stderr, "Failed to create window.");
		engine_quit();
		return 1;
	}

	/* Initialize OpenGL */
	if (!initOpenGL()) {
		fprintf(stderr, "Failed to create window.");
		engine_quit();
		return 1;
	}

	// Attach and compile shaders 
	shader *textShader = shader_loadFromFile("src/glsl/textShader.vert", "src/glsl/textShader.frag", TEXT);
	shader *S = shader_loadFromFile("src/glsl/shader.vert", "src/glsl/shader.frag", PHONG);

	if (S == NULL) {
		engine_quit();
		return 1;
	}

	/* Set the view matrix (camera) */
	camera *C = camera_init();

	/* Define the player */
	player* P = player_init(C->eye);

	/* Set the projection matrix */
	mat4x4 projection;
	mat4x4_perspective(projection, FOV, (float)WIDTH/(float)HEIGHT, 0.001f, 1000.f);

	vec3 center;
	vec3_mul_cross(C->up, C->right, C->direction);
	vec3_add(center, C->eye, C->direction);
	mat4x4_look_at(C->view, C->eye, center, C->up);

	/* Initialize all meshes */
	linkedList* meshList = list_create();

	level* mainLevel = level_loadMeshes("res/maps/map.bmp");
	list_insert(mainLevel->meshList, mesh_genFlatFloor(mainLevel->size, "res/textures/grass.jpg")); // CHECK FOR ERRORS

	// GUI
	mat4x4 ortho;
	mat4x4_ortho(ortho, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 100.0f);
	camera *GUI_Camera;	GUI_Camera = camera_init();
	mat4x4_identity(GUI_Camera->view);

	font* pixFont = font_load(12, 16, "res/fonts/pixfont.jpg");
	text *FPS = text_create("FPS:     ", pixFont, 1, 0, 0);

	SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);

	GLuint loc = glGetUniformLocation(textShader->program, "offset");
	GLuint loc1 = glGetUniformLocation(textShader->program, "projection");

	while (running) {
		pastTime = currentTime;
		currentTime = SDL_GetTicks();
		timePassed += (currentTime - pastTime);
		frameTime = (currentTime - pastTime);
		frames++;
		if (timePassed >= 1000) {
			timePassed = 0;
			char FPS_String[10];
			FPS_String[0] = 'F';
			FPS_String[1] = 'P';
			FPS_String[2] = 'S';
			FPS_String[3] = ':';
			FPS_String[4] = ' ';
			FPS_String[5] = (frames / 1000) % 10 + 48;
			FPS_String[6] = (frames / 100) % 10 + 48;
			FPS_String[7] = (frames / 10) % 10 + 48;
			FPS_String[8] = frames % 10 + 48;
			FPS_String[9] = 0;
			text_update(FPS, FPS_String);
			frames = 0;
		}

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT)
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					running = false;
			if (e.type == SDL_KEYDOWN) {
				printf("FKey Down\n");
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						mouseGrab = !mouseGrab;
						window_grabCursor(window, mouseGrab);
						break;
					case SDLK_q:
						running = false;
						break;
					case SDLK_a:
						P->movement.left = true;
						break;
					case SDLK_d:
						P->movement.right = true;
						break;
					case SDLK_w:
						printf("FORWARD\n");
						P->movement.forward = true;
						break;
					case SDLK_s:
						P->movement.backward = true;
						break;
					case SDLK_h:
						drawBoundingBox = !drawBoundingBox;
						break;
					case SDLK_LSHIFT:
						P->movement.run = true;
						break;
				}
			}
			if (e.type == SDL_KEYUP)
				switch(e.key.keysym.sym) {
					case SDLK_a:
						P->movement.left = false;
						break;
					case SDLK_d:
						P->movement.right = false;
						break;
					case SDLK_w:
						printf("NO MORE FORWARD\n");
						P->movement.forward = false;
						break;
					case SDLK_s:
						P->movement.backward = false;
						break;
					case SDLK_LSHIFT:
						P->movement.run = false;
						break;
				}
		}

		SDL_GetMouseState(&x, &y);
		
		/* Movement */
		camera_fps_movement(C, P, frameTime);
		player_setPosition(P, C->eye);

		/* FPS camera control */
		/*
		deltax = x - WIDTH/2;
		deltay = y - HEIGHT/2;

		if ((deltax != 0 || deltay != 0) && mouseGrab) {
			horizontalAngle += (float)(WIDTH/2 - x) * SENSITIVITY;
			verticalAngle += (float)(HEIGHT/2 - y) * SENSITIVITY;

			if (verticalAngle > PI/2.0f)
				verticalAngle = PI/2.0f;
			else if (verticalAngle < -PI/2.0f)
				verticalAngle = -PI/2.0f;

			SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
		}
		*/

		camera_fps_mouse_look(C, horizontalAngle, verticalAngle);
		vec3_add(center, C->eye, C->direction);
		mat4x4_look_at(C->view, C->eye, center, C->up);
		
		/* Rendering */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// GUI
		text_draw(FPS, textShader, ortho);

		mesh_drawList(mainLevel->meshList, S, C, projection);
		mesh_drawList(meshList, S, C, projection);
				
		// Update framebuffer
		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}
	
	// TODO: CLEAN UP;
	engine_quit();

	return 0;
}
