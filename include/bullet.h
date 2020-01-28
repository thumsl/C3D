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

bool bullet_maxDistance(bullet* B);
int bullet_updatePosition(bullet* B, unsigned int frameTime);
bullet* bullet_create(vec3 position, vec3 direction, bulletType* specs);
bulletType* bullet_createType(float speed, float damage, float maxTravel, const char* obj, const char* texture);
void bullet_destroy(bullet* B);

#endif // BULLET_H
