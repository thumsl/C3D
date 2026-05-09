#include "../include/player.h"
#include "../include/camera.h"
#include "../include/utils.h"
#include <stdlib.h>

// Walk speed in world units per millisecond. Matches the previous
// camera-driven movement (SPEED=0.05 units * frameTime ms in main.c).
static const float PLAYER_WALK_SPEED = 0.05f;

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

void player_update(C3D_Player *P, double dt)
{
	if (P->camera == NULL)
		return;

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

		float factor = PLAYER_WALK_SPEED * (float)dt;

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

	// Refresh the hitbox to match the new feet position.
	player_setPosition(P, P->position);

	// Sync camera eye to head position. camera_update remains the
	// caller's responsibility (matches the existing main.c pattern).
	P->camera->eye[0] = P->position[0];
	P->camera->eye[1] = P->position[1] + P->height;
	P->camera->eye[2] = P->position[2];
}
