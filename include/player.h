#ifndef PLAYER_H
#define PLAYER_H
#include "linmath.h"
#include "mesh.h"
#include <stdbool.h>

typedef struct {
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool run;
} playerMovement;

typedef struct {
	vec3 position;
	playerMovement movement;
	boundingBox hitbox;
} player;

player* player_init(vec3 position);

void player_updateHitbox(player* P, vec3 position);
// TODO: destroy Player
#endif