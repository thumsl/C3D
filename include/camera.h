#ifndef CAMERA_H
#define CAMERA_H
#include "linmath.h"
#include "player.h"

typedef struct player player;

typedef struct camera {
	vec3 eye;
	vec3 direction;
	vec3 right;
	vec3 up;
	mat4x4 view;
} camera;

camera* camera_init();
void camera_fps_mouse_look(camera* C, float horizontalAngle, float verticalAngle);
void camera_fps_movement(camera* C, player *P, unsigned short frameTime);
void camera_copy(camera *a, camera *b);

#endif // CAMERA_H	
