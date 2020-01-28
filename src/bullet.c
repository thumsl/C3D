#include "../include/bullet.h"
#include "../include/mesh.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

bullet* bullet_create(vec3 position, vec3 direction, bulletType *specs) {
	bullet* ret = malloc(sizeof(bullet));
	
	ret->specs = specs;
	ret->distanceTraveled = 0;
	vec3_copy(ret->direction, direction);
	vec3_copy(ret->position, position);
	ret->frameDistance = ret->specs->speed * sqrt(ret->direction[0]*ret->direction[0] + ret->direction[1]*ret->direction[1] + ret->direction[2]*ret->direction[2]);

	ret->model = malloc(sizeof(mesh));
	memcpy(ret->model, ret->specs->model, sizeof(mesh));

	mesh_translate(ret->model, position[0], position[1], position[2]);

	return ret;
}

bulletType* bullet_createType(float speed, float damage, float maxTravel, const char* obj, const char* texture) {
	bulletType* ret = malloc(sizeof(bulletType));

	ret->baseDamage = damage;
	ret->speed = speed;
	ret->maximumTravelDistance = maxTravel;
	ret->model = mesh_loadFromFile(obj, texture);

	return ret;
}

int bullet_updatePosition(bullet* B, unsigned int frameTime) {
	B->distanceTraveled += frameTime * B->frameDistance;
	vec3 velocity;
	vec3_scale(velocity, B->direction, frameTime * B->specs->speed);
	B->position[0] += velocity[0];
	B->position[1] += velocity[1];
	B->position[2] += velocity[2];
	mesh_translate_from_origin(B->model, B->position[0], B->position[1], B->position[2]);
	
	if (B->distanceTraveled < B->specs->maximumTravelDistance)
		return 1;
	else
		return 0;
}

bool bullet_maxDistance(bullet* B) {
	if (B->distanceTraveled > B->specs->maximumTravelDistance)
		return true;
	return false;
}

void bullet_destroy(bullet* B) {
	free(B->model);
}
