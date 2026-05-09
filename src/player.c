#include "../include/player.h"
#include "../include/camera.h"
#include "../include/level.h"
#include "../include/utils.h"
#include <math.h>
#include <stdlib.h>

// All physics in SI-ish units: meters, seconds, m/s, m/s^2.
// PLAYER_WALK_SPEED was 0.05 units/ms; the equivalent is 50.0 units/sec.
static const float PLAYER_WALK_SPEED  = 50.0f;       // units/sec
static const float PLAYER_GRAVITY     = -25.0f;      // units/sec^2 (y is up)
static const float PLAYER_JUMP_VEL    = 9.0f;        // units/sec
static const float PLAYER_STEP_HEIGHT = 0.4f;        // units; max snap-up tolerance

C3D_Player *player_init(vec3 position, float width, float height)
{
	C3D_Player *P = malloc(sizeof(C3D_Player));
	P->hitbox = malloc(sizeof(boundingBox));

	P->height = height;
	P->width = width;

	P->velocity[0] = 0.0f;
	P->velocity[1] = 0.0f;
	P->velocity[2] = 0.0f;
	P->grounded = false;
	P->camera = NULL;

	P->movement.forward = false;
	P->movement.backward = false;
	P->movement.left = false;
	P->movement.right = false;
	P->movement.run = false;

	player_setPosition(P, position);

	return P;
}

void player_setPosition(C3D_Player *P, vec3 position)
{
	// Convention: P->position is the player's FEET position.
	// The hitbox is an axis-aligned box centered on x/z and rising
	// from feet (y) to feet+height.
	vec3_copy(P->position, position);

	const float half_w = P->width / 2.0f;

	P->hitbox->min[0] = P->position[0] - half_w;
	P->hitbox->min[1] = P->position[1];
	P->hitbox->min[2] = P->position[2] - half_w;

	P->hitbox->max[0] = P->position[0] + half_w;
	P->hitbox->max[1] = P->position[1] + P->height;
	P->hitbox->max[2] = P->position[2] + half_w;
}

void player_attachCamera(C3D_Player *P, C3D_Camera *C)
{
	P->camera = C;

	// Sync camera eye to head position immediately so the view is
	// consistent before the first player_update call.
	P->camera->eye[0] = P->position[0];
	P->camera->eye[1] = P->position[1] + P->height;
	P->camera->eye[2] = P->position[2];
}

void player_update(C3D_Player *P, level *L, double dt)
{
	if (P->camera == NULL)
		return;

	// Caller passes dt in milliseconds (existing main.c convention).
	// All physics below is in seconds.
	float dt_s = (float)(dt * 0.001);

	C3D_Movement *M = &P->movement;

	if (M->forward || M->backward || M->right || M->left) {
		// Project the camera direction onto the XZ plane so walking
		// stays horizontal regardless of pitch.
		vec3 forward = { P->camera->direction[0], 0.0f, P->camera->direction[2] };
		float flen = vec3_len(forward);
		if (flen > 1e-6f)
			vec3_scale(forward, forward, 1.0f / flen);

		// camera->right is already horizontal (right[1] = 0 in camera_update_angle).
		vec3 right = { P->camera->right[0], P->camera->right[1], P->camera->right[2] };

		float factor = PLAYER_WALK_SPEED * dt_s;

		vec3 step;
		if (M->forward) {
			vec3_scale(step, forward, factor);
			vec3_add(P->position, P->position, step);
		}
		if (M->backward) {
			vec3_scale(step, forward, factor);
			vec3_sub(P->position, P->position, step);
		}
		if (M->right) {
			vec3_scale(step, right, factor);
			vec3_add(P->position, P->position, step);
		}
		if (M->left) {
			vec3_scale(step, right, factor);
			vec3_sub(P->position, P->position, step);
		}
	}

	// Gravity + ground snap. When L is NULL we have no collision world
	// yet, so skip gravity entirely (matches Phase 1 floating behavior).
	if (L != NULL) {
		P->velocity[1] += PLAYER_GRAVITY * dt_s;
		P->position[1] += P->velocity[1] * dt_s;

		float ground_y = level_groundHeightAt(L, P->position[0], P->position[2],
						      P->position[1] + PLAYER_STEP_HEIGHT);

		if (ground_y == -INFINITY) {
			// No support beneath the player — keep falling.
			P->grounded = false;
		} else if (P->position[1] < ground_y) {
			P->position[1] = ground_y;
			P->velocity[1] = 0.0f;
			P->grounded = true;
		} else {
			P->grounded = false;
		}
	}

	// Refresh the hitbox to match the new feet position.
	player_setPosition(P, P->position);

	// Sync camera eye to head position. camera_update remains the
	// caller's responsibility (matches the existing main.c pattern).
	P->camera->eye[0] = P->position[0];
	P->camera->eye[1] = P->position[1] + P->height;
	P->camera->eye[2] = P->position[2];
}

void player_jump(C3D_Player *P)
{
	if (P->grounded) {
		P->velocity[1] = PLAYER_JUMP_VEL;
		P->grounded = false;
	}
}
