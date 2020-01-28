#include "../include/player.h"
#include "../include/c3d.h"
#include "../include/utils.h"
#include <stdlib.h>

player* player_init(vec3 position, float width, float height) {
	player *P = (player*)malloc(sizeof(player));
	P->hitbox = (boundingBox*)malloc(sizeof(boundingBox));
	player_setPosition(P, position);

	P->movement.forward 	= false;
	P->movement.backward 	= false;
	P->movement.left 		= false;
	P->movement.right 		= false;
	P->movement.run 		= false;

	P->height = height;
	P->width = width;

	return P;
}

void player_setPosition(player* P, vec3 position) {
	vec3_copy(P->position, position);

	P->hitbox->min[0] = P->position[0] - P->width/2.0f; // optimize
	P->hitbox->min[1] = P->position[1] - P->height;
	P->hitbox->min[2] = P->position[2];

	P->hitbox->max[0] = P->position[0] + P->width/2.0f;
	P->hitbox->max[1] = P->position[1];
	P->hitbox->max[2] = P->position[2] + P->height;
}
