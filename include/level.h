#ifndef LEVEL_H
#define LEVEL_H

#include "list.h"
#define WALL 0xffffffff // WHITE

typedef struct level {
	unsigned int size;
	linkedList *meshList;
} level;

level* level_loadMeshes(const char *path);

#endif // LEVEL_H
