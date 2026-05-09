#ifndef PLAYER_H
#define PLAYER_H

#include "aabb.h"
#include "linmath.h"
#include <stdbool.h>

typedef struct boundingBox boundingBox;

// Forward-declare camera here to avoid a circular include with camera.h
// (camera.h already includes player.h). The player only holds a non-owning
// pointer, so the full definition isn't required at this point.
typedef struct camera C3D_Camera;

typedef struct movement {
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool run;
} C3D_Movement;

typedef struct {
	vec3 position;
	vec3 velocity;
	C3D_Movement movement;
	boundingBox *hitbox;
	C3D_Camera *camera;
	float height;
	float width;
	bool grounded;
} C3D_Player;

C3D_Player *player_init(vec3 position, float w, float h);
void player_setPosition(C3D_Player *P, vec3 position);
void player_attachCamera(C3D_Player *P, C3D_Camera *C);
void player_update(C3D_Player *P, double dt);

// TODO: destroy Player
#endif
