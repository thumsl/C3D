#ifndef BULLET_H
#define BULLET_H
#include "mesh.h"
#include "linmath.h"

typedef struct {
	vec3 position;
	vec3 velocity;
	mesh *M;
} bullet;

bullet* createBullet(vec3 postion, vec3 velocity, mesh* bulletMesh);

#endif // BULLET_H