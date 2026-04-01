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
} C3D_Movement;

typedef struct {
	vec3 position;
	C3D_Movement movement;
	boundingBox *hitbox;
	float height;
	float width;
} C3D_Player;

C3D_Player *player_init(vec3 position, float w, float h);
void player_setPosition(C3D_Player *P, vec3 position);

// TODO: destroy Player
#endif
