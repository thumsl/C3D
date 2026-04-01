#include "../include/camera.h"
#include "../include/c3d.h"
#include <math.h>

camera *camera_init(vec3 pos, float yaw, float pitch)
{
	//todo: destroy camera
	camera *C = malloc(sizeof(camera));
	C->yaw = yaw;
	C->pitch = pitch;

	camera_update_angle(C);
	vec3_copy(C->eye, pos);

	return C;
}

void camera_update_angle(camera *C)
{
	C->direction[0] = cosf(C->pitch) * sinf(C->yaw);
	C->direction[1] = sinf(C->pitch);
	C->direction[2] = cosf(C->pitch) * cosf(C->yaw);

	C->right[0] = sinf(C->yaw - C3D_PI / 2.0f);
	C->right[1] = 0;
	C->right[2] = cosf(C->yaw - C3D_PI / 2.0f);

	vec3_mul_cross(C->up, C->right, C->direction);
}

void camera_move(camera *C, C3D_Movement *M, double factor)
{
	if (M->forward || M->backward || M->right || M->left) {
		vec3 scaled_direction, scaled_right;

		vec3_scale(scaled_direction, C->direction, factor);
		vec3_scale(scaled_right, C->right, factor);

		if (M->forward)
			vec3_add(C->eye, C->eye, scaled_direction);
		if (M->backward)
			vec3_sub(C->eye, C->eye, scaled_direction);
		if (M->right)
			vec3_add(C->eye, C->eye, scaled_right);
		if (M->left)
			vec3_sub(C->eye, C->eye, scaled_right);
	}
}

void camera_copy(camera *a, camera *b)
{
	vec3_copy(a->eye, b->eye);
	vec3_copy(a->direction, b->direction);
	vec3_copy(a->right, b->right);
	vec3_copy(a->up, b->up);
}

void camera_update(camera *C)
{
	// printf("Eye:\t\t %f %f %f\n", C->eye[0], C->eye[1], C->eye[2]);
	// printf("Direction:\t\t %f %f %f\n", C->direction[0], C->direction[1], C->direction[2]);
	// printf("Right:\t\t %f %f %f\n\n", C->right[0], C->right[1], C->right[2]);

	vec3 center;
	vec3_mul_cross(C->up, C->right, C->direction);
	vec3_add(center, C->eye, C->direction);
	mat4x4_look_at(C->view, C->eye, center, C->up);
}
