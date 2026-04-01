#ifndef CAMERA_H
#define CAMERA_H

#include "linmath.h"
#include "player.h"

typedef struct movement C3D_Movement;

typedef struct camera {
	vec3 eye;
	vec3 direction;
	vec3 right;
	vec3 up;
	mat4x4 view;

	float yaw;
	float pitch;
} C3D_Camera;

C3D_Camera *camera_init(vec3 pos, float yaw, float pitch);
void camera_update_angle(C3D_Camera *C);

void camera_move(C3D_Camera *C, C3D_Movement *M, double factor);
void camera_copy(C3D_Camera *a, C3D_Camera *b);
void camera_update(C3D_Camera *C);

#endif // CAMERA_H
