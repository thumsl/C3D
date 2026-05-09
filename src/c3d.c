#include "../include/c3d.h"
#include "SDL2/SDL.h"

static C3D_Key_Event_Callbacks c3d_key_events = { 0 };

static C3D_TextureQuality g_texture_quality = C3D_TEXTURE_QUALITY_HIGH;

void c3d_set_texture_quality(C3D_TextureQuality q)
{
	g_texture_quality = q;
}

C3D_TextureQuality c3d_get_texture_quality(void)
{
	return g_texture_quality;
}

bool initOpenGL()
{
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		fprintf(stderr, "Error intializing GLEW: %s\n", glewGetErrorString(status));
		return 0;
	} else {
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		printf("GL version: %s\n", glGetString(GL_VERSION));
		return 1;
	}
}

void c3d_quit()
{
	SDL_Quit();
	exit(EXIT_FAILURE);
}

C3D_Game *c3d_init(const char *title, int width, int height, int options)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return NULL;
	}

	C3D_Game *game = calloc(1, sizeof(C3D_Game));
	if (!game) {
		fprintf(stderr, "Failed to allocate memory for game\n");
		c3d_quit();
	}

	if (width <= 0 || height <= 0) {
		game->window = window_create_fullsize(title);
	} else {
		game->window = window_create(width, height, title);
	}

	if (options & C3D_OPTION_FULLSCREEN) {
		window_fullscreen(game->window, true);
	}

	if (!game->window) {
		free(game);
		c3d_quit();
	}

	if (!initOpenGL()) {
		//c3d_destroy_window(game->window); TODO
		free(game);
		c3d_quit();
	}

	game->should_quit = false;
	game->cursor_grabbed = true;
	game->mouse_sensitivity = 0.01f;
	game->mouse1_pressed = false;
	game->mouse2_pressed = false;
	c3d_key_events.userdata = game;
	return game;
}

void c3d_set_key_callback(int key, C3D_Key_State state, C3D_key_callback callback)
{
	if (key < 0 || key >= C3D_MAX_KEYBOARD_KEYS) {
		fprintf(stderr, "Key code %d is out of bounds\n", key);
		return;
	}

	if (state == C3D_KEY_PRESSED) {
		c3d_key_events.on_press[key] = callback;
	} else if (state == C3D_KEY_RELEASED) {
		c3d_key_events.on_release[key] = callback;
	} else {
		fprintf(stderr, "Invalid key state: %d\n", state);
	}
}

void c3d_dispatch_key_event(int key, C3D_Key_State state)
{
	if (key < 0 || key >= C3D_MAX_KEYBOARD_KEYS) {
		fprintf(stderr, "Key code %d is out of bounds\n", key);
		return;
	}

	if (state == C3D_KEY_PRESSED) {
		if (c3d_key_events.on_press[key] == NULL) {
			return;
		}
		c3d_key_events.on_press[key](c3d_key_events.userdata);
	} else if (state == C3D_KEY_RELEASED) {
		if (c3d_key_events.on_release[key] == NULL) {
			return;
		}
		c3d_key_events.on_release[key](c3d_key_events.userdata);
	} else {
		fprintf(stderr, "Invalid key state: %d\n", state);
	}
}

void _update_mouse_position(C3D_Game *game, SDL_MouseMotionEvent *motion)
{
	if (game->cursor_grabbed) {
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);

		float deltax = mouse_x - game->window->width / 2;
		float deltay = mouse_y - game->window->height / 2;

		if (deltax != 0 || deltay != 0) {
			game->camera->yaw += (float)(game->window->width / 2 - mouse_x) * game->mouse_sensitivity;
			game->camera->pitch += (float)(game->window->height / 2 - mouse_y) * game->mouse_sensitivity;

			if (game->camera->pitch > C3D_PI / 2.0f)
				game->camera->pitch = C3D_PI / 2.0f;
			else if (game->camera->pitch < -C3D_PI / 2.0f)
				game->camera->pitch = -C3D_PI / 2.0f;

			SDL_WarpMouseInWindow(game->window->window, game->window->width / 2, game->window->height / 2);
			camera_update_angle(game->camera);
		}
	}
}

void c3d_process_input(C3D_Game *game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			game->should_quit = true;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				game->window->width = event.window.data1;
				game->window->height = event.window.data2;
				glViewport(0, 0, game->window->width, game->window->height);
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.repeat)
				c3d_key_events.is_pressed[event.key.keysym.scancode] = true;
			c3d_dispatch_key_event(event.key.keysym.scancode, C3D_KEY_PRESSED);

			switch (event.key.keysym.sym) {
			case SDLK_w:
				game->movement->forward = true;
				break;
			case SDLK_s:
				game->movement->backward = true;
				break;
			case SDLK_d:
				game->movement->right = true;
				break;
			case SDLK_a:
				game->movement->left = true;
				break;
			}
			break;

		case SDL_KEYUP:
			c3d_dispatch_key_event(event.key.keysym.scancode, C3D_KEY_RELEASED);
			switch (event.key.keysym.sym) {
			case SDLK_w:
				game->movement->forward = false;
				break;
			case SDLK_s:
				game->movement->backward = false;
				break;
			case SDLK_d:
				game->movement->right = false;
				break;
			case SDLK_a:
				game->movement->left = false;
				break;
			}

			break;
		case SDL_MOUSEMOTION:
			_update_mouse_position(game, &event.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				game->mouse1_pressed = true;
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				game->mouse2_pressed = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				game->mouse1_pressed = false;
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				game->mouse2_pressed = false;
			}
			break;
		}
	}

	// If a player is attached, mirror the engine's movement state into it
	// so the user doesn't have to copy it by hand each frame.
	if (game->player != NULL) {
		game->player->movement = *game->movement;
	}
}

void c3d_toggle_grab_cursor(C3D_Game *game)
{
	game->cursor_grabbed = !game->cursor_grabbed;
	SDL_SetRelativeMouseMode(game->cursor_grabbed ? SDL_TRUE : SDL_FALSE);
	window_grab_cursor(game->window, game->cursor_grabbed);
	SDL_WarpMouseInWindow(game->window->window, game->window->width / 2, game->window->height / 2);
}
