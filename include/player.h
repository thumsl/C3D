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
} player_movement_type;

typedef struct {
	vec3 position;
	player_movement_type movement;
	bounding_box hitbox;
} player;

player* initPlayer(vec3 position);

void updateHitbox(player* P, vec3 position);
// TODO: destroy Player
#endif