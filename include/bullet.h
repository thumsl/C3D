#ifndef BULLET_H
#define BULLET_H
#include "mesh.h"
#include "linmath.h"

typedef struct bulletType {
	float baseDamage;
	float speed;
	float maximumTravelDistance;
	mesh* model;
} bulletType;

typedef struct bullet {
	vec3 direction;
	vec3 position;
	mesh* model;
	float distanceTraveled;
	float frameDistance;
	bulletType *specs;
} bullet;

bullet* bullet_create(vec3 position, vec3 direction, bulletType* specs);
bulletType* bullet_createType(float speed, float damage, float maxTravel, const char* obj, const char* texture);
int bullet_updatePosition(bullet* B, unsigned int frameTime);

#endif // BULLET_H
