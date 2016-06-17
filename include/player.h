#ifndef PLAYER_H
#define PLAYER_H
#include "linmath.h"
#include "mesh.h"

typedef struct {
	short forward;
	short backward;
	short left;
	short right;
	short run;
} player_movement;

typedef struct {
	vec3 position;
	player_movement movement;
	bounding_box hitbox;
} player;

player* initPlayer(vec3 position);

void updateHitbox(player* P, vec3 position);
// TODO: destroy Player
#endif