#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include "linmath.h"
#include "mesh.h"
#include "render.h"
#include "utils.h"
#include "window.h"

#define WIDTH 1280
#define HEIGHT 720
#define PI 3.14159265359f
#define FOV 60.0f * 0.0174533f
#define SENSITIVITY 0.001f
#define STEP 0.01f
#define FULLSCREEN 0 // SDL_FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP (fake fullscreen) // 0 Windowed

void quit();

#endif