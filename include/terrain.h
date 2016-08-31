#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

typedef struct terrain {
	mesh *model;
	float **heightMap;
	int size;
} terrain;

void terrain_destroy(terrain *toDestroy);
terrain* terrain_genDiamondSquare(int size, float range, float factor, const char* texturePath);

#endif // TERRAIN_H