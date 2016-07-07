#ifndef AABB_H
#define AABB_H
#include "linmath.h"

typedef struct {
	vec3 min;
	vec3 max;
} bounding_box;

int aabb_collision(bounding_box a, bounding_box b);

#endif // AABB_H