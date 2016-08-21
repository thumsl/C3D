#ifndef PLAYER_H
#define PLAYER_H

#include "aabb.h"
#include "linmath.h"
#include <stdbool.h>

typedef struct boundingBox_s boundingBox;

typedef struct playerMovement_s {
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool run;
} playerMovement;

typedef struct player_s {
	vec3 position;
	playerMovement movement;
	boundingBox* hitbox;
} player;

player* player_init(vec3 position);
void player_setPosition(player* P, vec3 position);

// TODO: destroy Player
#endif