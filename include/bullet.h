#ifndef BULLET_H
#define BULLET_H
#include "mesh.h"
#include "linmath.h"

typedef struct {
	mesh *M;
	vec3 velocity;
	vec3 position;
} bullet;

bullet* bullet_create(vec3 position, vec3 velocity, const char* bulletMesh);
void bullet_updatePosition(bullet* B, unsigned int frameTime);

#endif // BULLET_H