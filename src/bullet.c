#include "../include/bullet.h"
#include "../include/utils.h"
#include <stdlib.h>

bullet* createBullet(vec3 position, vec3 velocity, mesh* bulletMesh) {
	bullet* ret = (bullet*)malloc(sizeof(bullet));
	vec3_copy(ret->velocity, velocity);
	vec3_copy(ret->velocity, position);
	ret->M = bulletMesh;

	return ret;
}