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

void camera_fps_move(camera* C, vec4 playerDirection, unsigned short frameTime) {
	vec3 scaled_direction, scaled_right;
	vec3_scale(scaled_direction, C->direction, WALK * frameTime);
	vec3_scale(scaled_right, C->right, WALK*frameTime);

	// TODO: define constants for FORWARD RIGHT BACKWARDS LEFT
	if (playerDirection[0]) // forward
		vec3_add(C->eye, C->eye, scaled_direction);
	if (playerDirection[1]) // backwards
		vec3_sub(C->eye, C->eye, scaled_direction);
	if (playerDirection[2]) // right
		vec3_add(C->eye, C->eye, scaled_right);
	if (playerDirection[3]) // left
		vec3_sub(C->eye, C->eye, scaled_right);
}

void camera_fps_move_simulate(vec3 R, camera* C, vec4 playerDirection, unsigned short frameTime) {
	vec3_copy(R, C->eye);
	vec3 scaled_direction, scaled_right;
	vec3_scale(scaled_direction, C->direction, WALK * frameTime);
	vec3_scale(scaled_right, C->right, WALK*frameTime);

	// TODO: define constants for FORWARD RIGHT BACKWARDS LEFT
	if (playerDirection[0]) // forward
		vec3_add(R, R, scaled_direction);
	if (playerDirection[1]) // backwards
		vec3_sub(R, R, scaled_direction);
	if (playerDirection[2]) // right
		vec3_add(R, R, scaled_right);
	if (playerDirection[3]) // left
		vec3_sub(R, R, scaled_right);
}

