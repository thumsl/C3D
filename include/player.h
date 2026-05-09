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

// Forward-declare level to avoid pulling in level.h (and therefore list.h)
// from this header. Only a pointer is needed at the API surface.
typedef struct level level;

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

// Attach the camera the player should drive. Must be called before
// player_update; without an attached camera, player_update is a no-op
// (the camera supplies the facing direction used to translate WASD
// into world-space motion).
void player_attachCamera(C3D_Player *P, C3D_Camera *C);

// Advance the player one frame. dt is in MILLISECONDS, matching the
// frameTime convention used elsewhere in the engine. Pass L = NULL to
// disable gravity and ground collision (e.g. for menus, free-cam, or
// any scene without a level); pass a level to enable physics.
void player_update(C3D_Player *P, level *L, double dt);

// Apply a jump impulse. No-op unless the player is currently grounded.
void player_jump(C3D_Player *P);

// TODO: destroy Player
#endif
