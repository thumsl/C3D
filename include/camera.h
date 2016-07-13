#ifndef CAMERA_H
#define CAMERA_H
#include "linmath.h"
#include "player.h"

typedef struct {
	vec3 eye;
	vec3 direction;
	vec3 right;
	vec3 up;
} camera;

camera* camera_init();
void camera_fps_mouse_look(camera* C, float horizontalAngle, float verticalAngle);
void camera_fps_movement(camera* C, playerMovement movement, unsigned short frameTime);
void camera_fps_movement_simulate(vec3 R, camera* C, playerMovement movement, unsigned short frameTime);

#endif // CAMERA_H	