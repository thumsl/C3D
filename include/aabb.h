#ifndef AABB_H
#define AABB_H

#include "linmath.h"
#include "mesh.h"

typedef struct mesh mesh;

typedef struct boundingBox {
	vec3 min;
	vec3 max;
} boundingBox;

int aabb_collision(boundingBox *a, boundingBox *b);
int aabb_collisionMeshes(mesh *a, mesh *b);

#endif // AABB_H
