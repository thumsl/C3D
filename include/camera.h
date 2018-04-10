#ifndef CAMERA_H
#define CAMERA_H
#include "linmath.h"
#include "player.h"

typedef struct Movement Movement;
typedef struct player player;

typedef struct camera {
	vec3 eye;
	vec3 direction;
	vec3 right;
	vec3 up;
	mat4x4 view;
} camera;

camera* camera_init(vec3 pos, float horizontalAngle, float verticalAngle);
void camera_angle(camera* C, float horizontalAngle, float verticalAngle);
void camera_move(camera *C, Movement* M, double factor);
void camera_copy(camera *a, camera *b);
void camera_update(camera *C);

#endif // CAMERA_H	
