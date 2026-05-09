#ifndef LEVEL_H
#define LEVEL_H

#include "list.h"
#define WALL 0xffffff // WHITE (RGB, 3 bytes per pixel)
// TODO: move this to the wall? struct
#define WALL_WIDTH 1
#define WALL_HEIGHT 3

typedef struct level {
	unsigned int size;
	linkedList *meshList;
} level;

level *level_loadMeshes(const char *path, const char *texture_path);

// Returns the highest standable surface Y under (x, z) whose top is at most
// max_y, or -INFINITY if no mesh covers (x, z) within that ceiling.
// max_y lets the caller exclude obvious wall-tops above its current step
// height — without it, the player would teleport onto roofs.
float level_groundHeightAt(level *L, float x, float z, float max_y);

#endif // LEVEL_H
