#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

typedef struct terrain {
	mesh *model;
	float **heightMap;
	int size;
} terrain;

// Generate a diamond-square heightmap of side `size` (must be 2^n + 1).
// `smoothing_passes` runs that many 3x3 box-filter passes on the heightmap
// after the algorithm completes (edges clamped/extended); pass 0 to skip.
terrain *terrain_genDiamondSquare(int size, float range, float factor, int smoothing_passes, const char *texturePath);

// Sample the terrain surface at world (x, z) using bilinear interpolation.
// Returns -INFINITY when (x, z) is outside the terrain footprint. Used by
// player_update for ground-snap when a terrain has been attached to the
// player via player_setTerrain.
float terrain_heightAt(terrain *T, float x, float z);

void terrain_destroy(terrain *toDestroy);

#endif // TERRAIN_H
