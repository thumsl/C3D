#ifndef CAMERA_H
#define CAMERA_H
#include "linmath.h"

typedef struct {
	vec3 eye;
	vec3 direction;
	vec3 right;
	vec3 up;
} camera;

void camera_fps_mouse_look(camera* C, float horizontalAngle, float verticalAngle);
void camera_fps_move(camera* C, unsigned short mvup, unsigned short mvdown, unsigned short mvleft, unsigned short mvright, unsigned short frameTime);

#endif // CAMERA_H	