#ifndef ENGINE_H
#define ENGINE_H

#ifdef DEBUG
	# define DEBUG_PRINT(x) printf x
#else
	# define DEBUG_PRINT(x) 1==1
#endif

#include <stdbool.h>

#include "aabb.h"
#include "bullet.h"
#include "camera.h"
#include "font.h"
#include "level.h"
#include "light.h"
#include "linmath.h"
#include "list.h"
#include "mesh.h"
#include "player.h"
#include "shader.h"
#include "phongShader.h"
#include "textShader.h"
#include "terrain.h"
#include "text.h"
#include "utils.h"
#include "window.h"

#define WIDTH 800
#define HEIGHT 600
#define PI 3.14159265359f
#define FOV 90.0f * 0.0174533f
#define SENSITIVITY 0.001f
#define WALK 0.003f
#define RUN 0.03f
#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH 1
#define WALL_HEIGHT 2.0f
#define WALL_WIDTH 0.3f

void engine_quit();
bool initOpenGL();

#endif
