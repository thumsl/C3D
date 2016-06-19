#ifndef ENGINE_H
#define ENGINE_H

#ifdef DEBUG
	# define DEBUG_PRINT(x) printf x
#else
	# define DEBUG_PRINT(x) 1==1
#endif

#include "camera.h"
#include "light.h"
#include "linmath.h"
#include "mesh.h"
#include "player.h"
#include "render.h"
#include "shader.h"
#include "utils.h"
#include "window.h"

#define WIDTH 800
#define HEIGHT 600
#define PI 3.14159265359f
#define FOV 60.0f * 0.0174533f
#define SENSITIVITY 0.001f
#define WALK 0.01f
#define RUN 0.1f
#define FULLSCREEN 0 // SDL_FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP (fake fullscreen) // 0 Windowed
#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH 1

void quit();

#endif