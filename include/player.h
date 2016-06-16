#ifndef PLAYER_H
#define PLAYER_H
#include "linmath.h"
#include "mesh.h"

typedef struct {
	float height;
	float width;	// rectangle
	vec3 position; // does position need to be stored?
	vec4 direction; // velocity[0] == 1 forward, velocity[0] == 0 backwards, velocity[1] == 1 right, velocity[1] == 0 left;
	bounding_box hitbox;
} player;

player* initPlayer(vec3 position, float height, float width);

void updateHitbox(player* P, vec3 position);
// TODO: destroy Player
#endif