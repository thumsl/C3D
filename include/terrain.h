#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

typedef struct terrain {
	mesh *model;
	float **heightMap;
	int size;
} terrain;

terrain* terrain_genDiamondSquare(int size, float range, float factor, const char* texturePath);
void terrain_destroy(terrain *toDestroy);

#endif // TERRAIN_H
