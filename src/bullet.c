#include "../include/bullet.h"
#include "../include/utils.h"
#include <stdlib.h>

#define BULLET_BASE_SPEED 0.01f

bullet* bullet_create(vec3 position, vec3 velocity, const char* bulletOBJFilename) {
	bullet* ret = (bullet*)malloc(sizeof(bullet));
	ret->M = initOBJMesh(bulletOBJFilename, "res/textures/test.png");
	mesh_translate(ret->M, position[0], position[1], position[2]);

	vec3_copy(ret->velocity, velocity);
	vec3_copy(ret->position, position);

	return ret;
}

void bullet_updatePosition(bullet* B, unsigned int frameTime) {
	vec3 vel;
	vec3_scale(vel, B->velocity, frameTime * BULLET_BASE_SPEED);
	B->position[0] += vel[0];
	B->position[1] += vel[1];
	B->position[2] += vel[2];
	mesh_translate_from_origin(B->M, B->position[0], B->position[1], B->position[2]);
}
