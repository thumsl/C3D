#include "../include/player.h"
#include "../include/engine.h"
#include "../include/utils.h"
#include <stdlib.h>

player* initPlayer(vec3 position) {
	player *P = (player*)malloc(sizeof(player));

	vec3_copy(P->position, position);

	P->movement.forward 	= 0;
	P->movement.backward 	= 0;
	P->movement.left 		= 0;
	P->movement.right 		= 0;
	P->movement.run 		= 0;

	return P;
}

void updateHitbox(player* P, vec3 position) {
	vec3_copy(P->position, position);

	P->hitbox.min[0] = P->position[0] - PLAYER_WIDTH/2.0f; // optimize
	P->hitbox.min[1] = P->position[1] - PLAYER_HEIGHT;
	P->hitbox.min[2] = P->position[2];

	P->hitbox.max[0] = P->position[0] + PLAYER_WIDTH/2.0f;
	P->hitbox.max[1] = P->position[1];
	P->hitbox.max[2] = P->position[2] + PLAYER_WIDTH;

	//DEBUG_PRINT(("Player: min vector = %f %f %f; max vector = %f %f %f\n", P->body.min[0], P->body.min[1], P->body.min[2], P->body.max[0], P->body.max[1], P->body.max[2]));
}