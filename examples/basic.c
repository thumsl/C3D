#include "include/bullet.h"
#include "include/c3d.h"
#include "include/camera.h"
#include "include/level.h"
#include "include/light.h"
#include "include/text.h"

#define SPEED 0.05
#define SENSITIVITY 0.002

#define FOV 120.0f * 0.0174533f

#define UPDATE_FPS_THRESHOLD 200

void close_game(void *data)
{
	printf("Closing game...\n");
	C3D_Game *game = (C3D_Game *)data;
	game->should_quit = true;
}

void configure_inputs()
{
	c3d_set_key_callback(SDL_SCANCODE_Q, C3D_KEY_PRESSED,
			     close_game); // Q to quit
}

int main(int argc, char *argv[])
{
	// Widht and Height == -1 means that the game will use the current monitor's resolution
	C3D_Game *mygame = c3d_init("Testing", -1, -1, C3D_OPTION_FULLSCREEN);
	configure_inputs();

	/* Attach and compile shaders */
	shader *S = shader_loadFromFile("src/glsl/shader.vert", "src/glsl/shader.frag", PHONG);
	shader *textShader = shader_loadFromFile("src/glsl/textShader.vert", "src/glsl/textShader.frag", TEXT);

	if (S == NULL) {
		c3d_quit();
		return 1;
	}

	bool mouseGrab = true, mouseMoved = false;
	float verticalAngle = -0.14f, horizontalAngle = C3D_PI;
	int mouse_x = mygame->window->width / 2, mouse_y = mygame->window->width / 2, pastTime = 0, currentTime = 0, frameTime = 0, frames_passed = 0,
	    initial_frame = 0;

	vec3 init_cam_position = { 3, 0.5f, -3.0f };
	mygame->camera = camera_init(init_cam_position, horizontalAngle, verticalAngle);

	mat4x4_perspective(mygame->projection, FOV, (float)mygame->window->width / (float)mygame->window->height, 0.001f, 1000.f);

	mat4x4_ortho(mygame->ortho, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 100.0f);

	// Ambient Light
	vec3 lightColor = { 1.0f, 1.0f, 1.0f }, lightPosition = { 0.0f, 35.0f, 0.0f };
	ambientLight *ambient = initAmbientLight(lightColor, 1.0f);
	setAmbientLight(ambient, S);

	lightColor[0] = 0;
	lightColor[2] = 0;
	pointLight *point = initPointLight(lightColor, init_cam_position, 2.0f, 2.0f);
	setPointLight(point, S); // S is the shader

	level *mainLevel = level_loadMeshes("res/maps/map.bmp", "res/textures/metal.jpg");
	list_insert(mainLevel->meshList, mesh_genFlatFloor(mainLevel->size,
							   "res/textures/grass.jpg")); // CHECK FOR ERRORS

	linkedList *meshList = list_create();
	mesh_loadFromFileToList("res/obj/spot.obj", "res/textures/spot.png", meshList);

	linkedList *bulletList = list_create();
	bulletType *bulletSpecs = bullet_createType(0.01, 100, 20, "res/obj/bullet.obj", "res/textures/metal.jpg");

	font *font = font_load(12, 16, "res/fonts/pixfont.jpg");
	text *text_msg = text_create("RANDOM TEXT MESSAGE", font, 3, 0.2, 0.95);
	text *fps_counter_label = text_create("FPS    ", font, 2, 0, 0);

	mygame->movement = malloc(sizeof(C3D_Movement));
	mygame->movement->forward = mygame->movement->backward = mygame->movement->right = mygame->movement->left = false;
	bool show_fps = false;

	SDL_WarpMouseInWindow(mygame->window->window, mygame->window->width / 2, mygame->window->height / 2);

	while (mygame->should_quit == false) {
		frames_passed++;
		pastTime = currentTime;
		currentTime = SDL_GetTicks();
		frameTime = (currentTime - pastTime);

		if (show_fps && currentTime - initial_frame > UPDATE_FPS_THRESHOLD) {
			char fps_buffer[20];
			unsigned int fps = (int)(frames_passed * 1000.0f) / (currentTime - initial_frame);
			if (fps > 999)
				fps = 999;
			sprintf(fps_buffer, "%s%d", "FPS ", fps);
			text_update(fps_counter_label, fps_buffer);
			initial_frame = currentTime;
			frames_passed = 0;
		}

		c3d_process_input(mygame);

		// while (SDL_PollEvent(&e)) {
		// 	if (e.type == SDL_MOUSEBUTTONDOWN)
		// 		list_insert(bulletList,
		// 			    bullet_create(mygame->camera->eye, C->direction,
		// 					  bulletSpecs));
		// 	if (e.type == SDL_KEYDOWN)
		// 		switch (e.key.keysym.sym) {
		// 		case SDLK_ESCAPE:
		// 			mouseGrab = !mouseGrab;
		// 			mygame->window_grab_cursor(game->window,
		// 						   mouseGrab);
		// 			SDL_WarpMouseInWindow(
		// 				mygame->window->game->window,
		// 				mouse_x, mouse_y);
		// 			break;
		// 		case SDLK_q:
		// 			running = false;
		// 			break;
		// 		case SDLK_w:
		// 			movement->forward = true;
		// 			break;
		// 		case SDLK_s:
		// 			movement->backward = true;
		// 			break;
		// 		case SDLK_d:
		// 			movement->right = true;
		// 			break;
		// 		case SDLK_a:
		// 			movement->left = true;
		// 			break;
		// 		case SDLK_f:
		// 			show_fps = !(show_fps);
		// 			break;
		// 		}
		// 	else if (e.type == SDL_KEYUP)
		// 		switch (e.key.keysym.sym) {
		// 		case SDLK_w:
		// 			movement->forward = false;
		// 			break;
		// 		case SDLK_s:
		// 			movement->backward = false;
		// 			break;
		// 		case SDLK_d:
		// 			movement->right = false;
		// 			break;
		// 		case SDLK_a:
		// 			movement->left = false;
		// 			break;
		// 		}
		// 	else if (!mouseMoved && e.type == SDL_MOUSEMOTION)
		// 		mouseMoved = true;
		// }

		/* FPS camera control */
		if (mouseMoved && mouseGrab) {
			SDL_GetMouseState(&mouse_x, &mouse_y);

			float deltax = mouse_x - mygame->window->width / 2;
			float deltay = mouse_y - mygame->window->height / 2;

			if ((deltax != 0 || deltay != 0) && mouseGrab) {
				horizontalAngle += (float)(mygame->window->width / 2 - mouse_x) * SENSITIVITY;
				verticalAngle += (float)(mygame->window->height / 2 - mouse_y) * SENSITIVITY;

				if (verticalAngle > C3D_PI / 2.0f)
					verticalAngle = C3D_PI / 2.0f;
				else if (verticalAngle < -C3D_PI / 2.0f)
					verticalAngle = -C3D_PI / 2.0f;

				SDL_WarpMouseInWindow(mygame->window->window, mygame->window->width / 2, mygame->window->height / 2);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update point light's position
		point->position[0] = mygame->camera->eye[0];
		point->position[1] = mygame->camera->eye[1];
		point->position[2] = mygame->camera->eye[2];
		setPointLight(point, S);

		camera_angle(mygame->camera, horizontalAngle, verticalAngle);
		camera_move(mygame->camera, mygame->movement, SPEED * frameTime);
		camera_update(mygame->camera);

		if (show_fps)
			text_draw(fps_counter_label, textShader, mygame->ortho);
		text_draw(text_msg, textShader, mygame->ortho);

		// mesh_draw(floor, S, C, projection);
		mesh_drawList(mainLevel->meshList, S, mygame->camera, mygame->projection);
		mesh_drawList(meshList, S, mygame->camera, mygame->projection);

		// Draw bullets
		node *current_node = bulletList->head;
		while (current_node) {
			bullet *b = (bullet *)current_node->data;
			if (bullet_maxDistance(b)) {
				bullet_destroy(b);
				current_node = list_delete_node(bulletList, current_node);
				continue;
			}
			mesh_draw(b->model, S, mygame->camera, mygame->projection);
			bullet_updatePosition(b, frameTime);
			current_node = current_node->next;
		}

		SDL_GL_SwapWindow(mygame->window->window);
		SDL_Delay(1);
	}

	// TODO: Cleanup function
	free(mygame->movement);
	free(mygame->camera);
	free(S);
	free(meshList);
	free(ambient);

	c3d_quit();

	return EXIT_SUCCESS;
}
