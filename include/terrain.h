#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

typedef struct terrain {
	struct mesh *model;
	float **heightMap;
	int size;
} terrain;

void terrain_destroy(terrain *toDestroy);
static void terrain_diamondSquare(float** map, int size, float range, float factor);
terrain* terrain_genDiamondSquare(int size);

#endif // TERRAIN_H