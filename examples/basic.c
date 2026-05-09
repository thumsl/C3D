#include "include/bullet.h"
#include "include/c3d.h"
#include "include/camera.h"
#include "include/level.h"
#include "include/light.h"
#include "include/player.h"
#include "include/text.h"

#define SPEED 0.05

#define FOV 120.0f * 0.0174533f

#define UPDATE_FPS_THRESHOLD 200

static C3D_Game *mygame;
static bool show_fps = true;

void close_game(void *data)
{
	mygame->should_quit = true;
}

void toggle_grab_cursor(void *data)
{
	printf("Toggle grab cursor\n");
	c3d_toggle_grab_cursor(mygame);
}

void toggle_fps_display(void *data)
{
	printf("Toggle FPS display\n");
	show_fps = !show_fps;
}

void jump(void *data)
{
	player_jump(mygame->player);
}

void configure_inputs()
{
	c3d_set_key_callback(SDL_SCANCODE_Q, C3D_KEY_PRESSED, close_game); // Q to quit
	c3d_set_key_callback(SDL_SCANCODE_ESCAPE, C3D_KEY_PRESSED, toggle_grab_cursor); // Escape to toggle mouse grab
	c3d_set_key_callback(SDL_SCANCODE_F, C3D_KEY_PRESSED, toggle_fps_display); // F to toggle FPS display
	c3d_set_key_callback(SDL_SCANCODE_SPACE, C3D_KEY_PRESSED, jump); // Space to jump
}

int main(int argc, char *argv[])
{
	// Width and Height == -1 means that the game will use the current monitor's resolution
	mygame = c3d_init("Testing", -1, -1, C3D_OPTION_FULLSCREEN);
	configure_inputs();

	/* Attach and compile shaders */
	shader *S = shader_loadFromFile("src/glsl/shader.vert", "src/glsl/shader.frag", PHONG);
	shader *textShader = shader_loadFromFile("src/glsl/textShader.vert", "src/glsl/textShader.frag", TEXT);

	if (S == NULL) {
		c3d_quit();
		return 1;
	}

	bool mouseGrab = true, mouseMoved = false;
	int mouse_x = mygame->window->width / 2, mouse_y = mygame->window->width / 2, pastTime = 0, currentTime = 0, frameTime = 0, frames_passed = 0,
	    initial_frame = 0;

	// Walking-player setup. Skip these three lines (and call camera_move
	// instead of player_update below) for a free-flying camera.
	vec3 init_player_position = { 3.0f, 0.0f, -3.0f };       // feet position
	mygame->player = player_init(init_player_position, 0.6f, 1.8f); // width, height
	mygame->camera = camera_init(init_player_position, -0.14f, C3D_PI);
	player_attachCamera(mygame->player, mygame->camera);     // overrides camera eye to head height

	mat4x4_perspective(mygame->projection, FOV, (float)mygame->window->width / (float)mygame->window->height, 0.001f, 1000.f);

	mat4x4_ortho(mygame->ortho, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 100.0f);

	// Ambient Light
	vec3 lightColor = { 1.0f, 1.0f, 1.0f }, lightPosition = { 0.0f, 35.0f, 0.0f };
	ambientLight *ambient = initAmbientLight(lightColor, 1.0f);
	setAmbientLight(ambient, S);

	lightColor[0] = 0;
	lightColor[2] = 0;
	pointLight *point = initPointLight(lightColor, mygame->camera->eye, 2.0f, 2.0f);
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

	// Engine writes WASD state here; when a player is attached, the engine
	// also mirrors this into player->movement automatically.
	mygame->movement = malloc(sizeof(C3D_Movement));
	mygame->movement->forward = mygame->movement->backward = mygame->movement->right = mygame->movement->left = false;

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

		if (mygame->mouse1_pressed) {
			list_insert(bulletList, bullet_create(mygame->camera->eye, mygame->camera->direction, bulletSpecs));
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update point light's position
		point->position[0] = mygame->camera->eye[0];
		point->position[1] = mygame->camera->eye[1];
		point->position[2] = mygame->camera->eye[2];
		setPointLight(point, S);

		// player_update applies WASD + gravity + ground snap. Pass NULL
		// for the level to disable physics (menus, free-cam scenes).
		player_update(mygame->player, mainLevel, frameTime);
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
	free(mygame->player->hitbox);
	free(mygame->player);
	free(S);
	free(meshList);
	free(ambient);

	c3d_quit();

	return EXIT_SUCCESS;
}
