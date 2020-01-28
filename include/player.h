#ifndef PLAYER_H
#define PLAYER_H

#include "aabb.h"
#include "linmath.h"
#include <stdbool.h>

typedef struct boundingBox boundingBox;

typedef struct Movement {
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool run;
} Movement;

typedef struct player {
	vec3 position;
	Movement movement;
	boundingBox* hitbox;
	float height;
	float width;
} player;

player* player_init(vec3 position, float w, float h);
void player_setPosition(player* P, vec3 position);

// TODO: destroy Player
#endif
