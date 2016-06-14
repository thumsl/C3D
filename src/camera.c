#include "../include/camera.h"
#include "../include/engine.h"
#include <math.h>

void camera_fps_mouse_look(camera* C, float horizontalAngle, float verticalAngle) {
	C->direction[0] = cosf(verticalAngle) * sinf(horizontalAngle);
	C->direction[1] = sinf(verticalAngle);
	C->direction[2] = cosf(verticalAngle) * cosf(horizontalAngle);

	C->right[0] = sinf(horizontalAngle - PI/2.0f);
	C->right[1] = 0;
	C->right[2] = cosf(horizontalAngle - PI/2.0f);

	vec3_mul_cross(C->up, C->right, C->direction);
}

void camera_fps_move(camera* C, unsigned short mvup, unsigned short mvdown, unsigned short mvleft, unsigned short mvright) {
	vec3 scaled_direction, scaled_right;
	vec3_scale(scaled_direction, C->direction, STEP);
	vec3_scale(scaled_right, C->right, STEP);

	if (mvup)
		vec3_add(C->eye, C->eye, scaled_direction);
	if (mvdown)
		vec3_sub(C->eye, C->eye, scaled_direction);
	if (mvright)
		vec3_add(C->eye, C->eye, scaled_right);
	if (mvleft)
		vec3_sub(C->eye, C->eye, scaled_right);
}