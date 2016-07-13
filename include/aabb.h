#ifndef AABB_H
#define AABB_H
#include "linmath.h"

typedef struct {
	vec3 min;
	vec3 max;
} boundingBox;

int aabb_collision(boundingBox a, boundingBox b);

#endif // AABB_H