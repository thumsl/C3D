#include "../include/camera.h"
#include "../include/engine.h"
#include <math.h>

camera* camera_init() {
	//todo: destroy camera
	camera* C = (camera*)malloc(sizeof(camera));
	C->eye[0] = 0.0f; C->eye[1] = 1.5f; C->eye[2] = -2.0f;
	C->direction[0] = cosf(0) * sinf(PI); C->direction[1] = sinf(0); C->direction[2] = cosf(PI) * cosf(0);
	C->right[0] = sinf(PI - PI/2.0f); C->right[1] = 0.0f; C->right[2] =  cosf(PI - PI/2.0f);
}

void camera_fps_mouse_look(camera* C, float horizontalAngle, float verticalAngle) {
	C->direction[0] = cosf(verticalAngle) * sinf(horizontalAngle);
	C->direction[1] = sinf(verticalAngle);
	C->direction[2] = cosf(verticalAngle) * cosf(horizontalAngle);

	C->right[0] = sinf(horizontalAngle - PI/2.0f);
	C->right[1] = 0;
	C->right[2] = cosf(horizontalAngle - PI/2.0f);

	vec3_mul_cross(C->up, C->right, C->direction);
}

void camera_fps_movement(camera* C, player *P, unsigned short frameTime) {
	vec3 scaled_direction, scaled_right;

	if (P->movement.run) {
		vec3_scale(scaled_direction, C->direction, RUN * frameTime);
		vec3_scale(scaled_right, C->right, RUN * frameTime);
	} else {
		vec3_scale(scaled_direction, C->direction, WALK * frameTime);
		vec3_scale(scaled_right, C->right, WALK * frameTime);
	}

	// scaled_direction[1] = 0.0f;

	if (P->movement.forward)
		vec3_add(C->eye, C->eye, scaled_direction);
	if (P->movement.backward)
		vec3_sub(C->eye, C->eye, scaled_direction);
	if (P->movement.right)
		vec3_add(C->eye, C->eye, scaled_right);
	if (P->movement.left)
		vec3_sub(C->eye, C->eye, scaled_right);
}

void camera_copy(camera *a, camera *b) {
	vec3_copy(a->eye, b->eye);
	vec3_copy(a->direction, b->direction);
	vec3_copy(a->right, b->right);
	vec3_copy(a->up, b->up);
}