#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

// #include "aabb.h"
// #include "bullet.h"
// #include "camera.h"
// #include "font.h"
// #include "level.h"
// #include "light.h"
// #include "linmath.h"
// #include "list.h"
// #include "mesh.h"
// #include "player.h"
// #include "shader.h"
// #include "phongShader.h"
// #include "textShader.h"
// #include "terrain.h"
// #include "text.h"
// #include "utils.h"
// #include "window.h"
#include "SDL2/SDL.h"
#include "GL/glew.h"

#define C3D_PI 3.14159265359f

// TODO: all the constants below should be user-defined
#define WIDTH 1280
#define HEIGHT 720
#define FOV 90.0f * 0.0174533f
#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH 1

void c3d_quit();
bool initOpenGL();

#endif
