#ifndef PLAYER_H
#define PLAYER_H

#include "aabb.h"
#include "linmath.h"
#include <stdbool.h>

// TODO: these should come be a func parameter
#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH 1

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
} player;

player* player_init(vec3 position);
void player_setPosition(player* P, vec3 position);

// TODO: destroy Player
#endif
