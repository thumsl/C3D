#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "camera.h"
#include "window.h"

#define C3D_PI 3.14159265359f
#define C3D_MAX_KEYBOARD_KEYS SDL_NUM_SCANCODES
#define C3D_OPTION_FULLSCREEN 0x01

typedef enum { C3D_KEY_RELEASED, C3D_KEY_PRESSED } C3D_Key_State;

// Texture quality. HIGH enables mipmaps + trilinear + anisotropic filtering
// (when the GL extension is available). LOW preserves the original behavior:
// no mipmaps, plain GL_LINEAR. The setting is consulted at texture LOAD time;
// changing it does not affect already-uploaded textures.
typedef enum { C3D_TEXTURE_QUALITY_LOW, C3D_TEXTURE_QUALITY_HIGH } C3D_TextureQuality;
void c3d_set_texture_quality(C3D_TextureQuality q);
C3D_TextureQuality c3d_get_texture_quality(void);

typedef void (*C3D_key_callback)(void *userdata);

typedef struct {
	C3D_Window *window;
	C3D_Camera *camera;
	C3D_Player *player;
	C3D_Movement *movement;
	mat4x4 projection;
	mat4x4 ortho;
	bool should_quit;
	bool cursor_grabbed;
	float mouse_sensitivity;
	bool mouse1_pressed;
	bool mouse2_pressed;
} C3D_Game;

typedef struct {
	C3D_key_callback on_press[C3D_MAX_KEYBOARD_KEYS];
	C3D_key_callback on_release[C3D_MAX_KEYBOARD_KEYS];
	bool is_pressed[C3D_MAX_KEYBOARD_KEYS];
	void *userdata;
} C3D_Key_Event_Callbacks;

C3D_Game *c3d_init(const char *title, int width, int height, int options);
bool initOpenGL(void);
void c3d_quit();
void c3d_process_input(C3D_Game *game);
void c3d_set_key_callback(int key, C3D_Key_State state, C3D_key_callback callback);
void c3d_toggle_grab_cursor(C3D_Game *game);

#endif
