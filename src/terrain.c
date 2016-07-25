#include "../include/terrain.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <math.h>

void terrain_destroy(terrain *toDestroy) {
	free(toDestroy->model);
	free(toDestroy->heightMap);
	free(toDestroy);
}

static void terrain_diamondSquare(float** map, int size, float range, float factor) {
	int i, j, k;

	// NOTE: this is not correct, square values do not use the value of adjacent diamonds correctly
	for (i = size - 1; i >= 2; i /= 2, range *= pow(2,-factor)) {
		for (j = 0; j < size - 1; j += i) {
			for (k = 0; k < size - 1; k += i) {
				float a = access_2df_array(map, size, j, k);
				float b = access_2df_array(map, size, j, k+i);
				float c = access_2df_array(map, size, j+i, k);
				float d = access_2df_array(map, size, j+i, k+i);

				float e = access_2df_array(map, size, j + i/2, k + i/2) = (a + b + c + d)/4.0f + randomValue(range);

				access_2df_array(map, size, j, k+i/2) = (access_2df_array(map, size, j, k) + access_2df_array(map, size, j, k+i) + e)/3.0f + randomValue(range);
				access_2df_array(map, size, j+i/2, k) = (access_2df_array(map, size, j, k) + access_2df_array(map, size, j+i, k) + e)/3.0f + randomValue(range);
				access_2df_array(map, size, j+i, k+i/2) = (access_2df_array(map, size, j+i, k) + access_2df_array(map, size, j+i, k+i) + e)/3.0f + randomValue(range);
				access_2df_array(map, size, j+i/2, k+i) = (access_2df_array(map, size, j, k+i) + access_2df_array(map, size, j+i, k+i) + e)/3.0f + randomValue(range);
			}
		}
	}
}

terrain* terrain_genDiamondSquare(int size) {
    terrain* ret = (terrain*)malloc(sizeof(terrain));
    ret->model = NULL;
    ret->heightMap = (float**)malloc(sizeof(float) * size * size);
    ret->size = size;

	*((float*)ret->heightMap + 0 * size + 0) = 0;
	*((float*)ret->heightMap + 0 * size + (size-1)) = 0;
	*((float*)ret->heightMap + (size-1) * size + 0) = 0;
	*((float*)ret->heightMap + (size-1) * size + (size-1)) = 0;

    setRandSeed();
    terrain_diamondSquare(ret->heightMap, size, 50, 1.0);

    return ret;
}

