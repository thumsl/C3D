#ifndef LEVEL_H
#define LEVEL_H

#include "list.h"
#define WALL 0xffffffff // WHITE
// TODO: move this to the wall? struct
#define WALL_WIDTH 1 
#define WALL_HEIGHT 3

typedef struct level {
	unsigned int size;
	linkedList *meshList;
} level;

level* level_loadMeshes(const char *path, const char *texture_path);

#endif // LEVEL_H
