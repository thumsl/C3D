#include "include/c3d.h"
#include "include/window.h"
#include "include/camera.h"
#include "include/text.h"
#include "include/light.h"
#include "include/level.h"
#include "include/light.h"
#include <math.h>

#define SPEED 0.0035
#define SENSITIVITY 0.0025

int main(int argc, char* argv[]) {
	SDL_Window* window = window_create(WIDTH, HEIGHT, "Testing");

	if (window == NULL) {
		c3d_quit();
		return EXIT_FAILURE;
	}

	if (!initOpenGL()) {
		c3d_quit();
		return EXIT_FAILURE;
	}
	
	/* Attach and compile shaders */
	shader *S = shader_loadFromFile("src/glsl/shader.vert", "src/glsl/shader.frag", PHONG);
	shader *textShader = shader_loadFromFile("src/glsl/textShader.vert", "src/glsl/textShader.frag", TEXT);
	
	if (S == NULL) {
		c3d_quit();
		return 1;
	}

	SDL_Event e;
	bool running = true, mouseGrab = true, mouseMoved = false;
	float verticalAngle = -0.44f, horizontalAngle = C3D_PI, pastTime = 0, currentTime = 0, frameTime = 0;
	int mouse_x = WIDTH / 2, mouse_y = WIDTH / 2;
	
	vec3 init_cam_position = {2, 0.5f, -2.0f};
	camera *C = camera_init(init_cam_position, horizontalAngle, verticalAngle);

	mat4x4 projection;
	mat4x4_perspective(projection, FOV, (float)WIDTH/(float)HEIGHT, 0.001f, 1000.f);

	mat4x4 ortho;
	mat4x4_ortho(ortho, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 100.0f);

	// Ambient Light
	vec3 lightColor = {1.0f, 1.0f, 1.0f}, lightPosition = {0.0f, 35.0f, 0.0f};
	ambientLight *ambient = initAmbientLight(lightColor, 1.0f);
	setAmbientLight(ambient, S);

	// Spot light that follows the player
	lightColor[0] = 0; lightColor[2] = 0;
	pointLight *point = initPointLight(lightColor, init_cam_position, 2.0f, 2.0f);
	setPointLight(point, S); // S is the shader

	level* mainLevel = level_loadMeshes("res/maps/map.bmp", "res/textures/metal.jpg");
	list_insert(mainLevel->meshList, mesh_genFlatFloor(mainLevel->size, "res/textures/grass.jpg")); // CHECK FOR ERRORS

	// mesh* floor = mesh_genFlatFloor(16, "res/textures/grass.jpg");

	linkedList* meshList = list_create();
	mesh_loadFromFileToList("res/obj/spot.obj", "res/textures/spot.png", meshList);

	font* font = font_load(12, 16, "res/fonts/pixfont.jpg");
	text* fps_counter_label = text_create("FPS:		", font, 1, 0, 0);

	Movement* movement = (Movement*)malloc(sizeof(Movement));
	movement->forward = movement->backward = movement->right = movement->left = false;
	
	SDL_PumpEvents();
	SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);

	while (running) {
		pastTime = currentTime;
		currentTime = SDL_GetTicks();
		frameTime = (currentTime - pastTime);

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
				running = false;
			if (e.type == SDL_KEYDOWN)
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						mouseGrab = !mouseGrab;
						window_grabCursor(window, mouseGrab);
						SDL_WarpMouseInWindow(window, mouse_x, mouse_y);
						break;
					case SDLK_q:
						running = false;
						break;
					case SDLK_w:
						movement->forward = true;
						break;
					case SDLK_s:
						movement->backward = true;
						break;
					case SDLK_d:
						movement->right = true;
						break;
					case SDLK_a:
						movement->left = true;
						break;
				}
			else if (e.type == SDL_KEYUP)
				switch(e.key.keysym.sym) {
					case SDLK_w:
						movement->forward = false;
						break;
					case SDLK_s:
						movement->backward = false;
						break;
					case SDLK_d:
						movement->right = false;
						break;
					case SDLK_a:
						movement->left = false;
						break;
				}
			else if (!mouseMoved && e.type == SDL_MOUSEMOTION)
				mouseMoved = true;
		}

		/* FPS camera control */
		if (mouseMoved && mouseGrab) {
			SDL_GetMouseState(&mouse_x, &mouse_y);

			float deltax = mouse_x - WIDTH/2;
			float deltay = mouse_y - HEIGHT/2;
			
			if ((deltax != 0 || deltay != 0) && mouseGrab) {
				horizontalAngle += (float)(WIDTH/2 - mouse_x) * SENSITIVITY;
				verticalAngle += (float)(HEIGHT/2 - mouse_y) * SENSITIVITY;
			
				if (verticalAngle > C3D_PI/2.0f)
					verticalAngle = C3D_PI/2.0f;
				else if (verticalAngle < -C3D_PI/2.0f)
					verticalAngle = -C3D_PI/2.0f;
			
				SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update point light's position
		point->position[0] = C->eye[0];
		point->position[1] = C->eye[1];
		point->position[2] = C->eye[2];
		setPointLight(point, S);

		camera_angle(C, horizontalAngle, verticalAngle);
		camera_move(C, movement, SPEED * frameTime);
		camera_update(C);

		text_draw(fps_counter_label, textShader, ortho);

		//mesh_draw(floor, S, C, projection);
		mesh_drawList(mainLevel->meshList, S, C, projection);
	 	mesh_drawList(meshList, S, C, projection);

		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}

	// TODO: Cleanup function
	free(movement);
	free(C);
	free(S);
	free(meshList);
	free(ambient);

	c3d_quit();

	return EXIT_SUCCESS;
}
