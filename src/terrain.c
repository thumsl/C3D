#include "../include/terrain.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <math.h>

terrain* terrain_genDiamondSquare(int size, float range, float factor, const char* texturePath) {
    terrain* ret = malloc(sizeof(terrain));
    ret->model = NULL;
    ret->heightMap = malloc(sizeof(float) * size * size);
    ret->size = size;

	*((float*)ret->heightMap + 0 * size + 0) = 0;
	*((float*)ret->heightMap + 0 * size + (size-1)) = 0;
	*((float*)ret->heightMap + (size-1) * size + 0) = 0;
	*((float*)ret->heightMap + (size-1) * size + (size-1)) = 0;

    setRandSeed();
	int i, j, k;

	// NOTE: this is not correct, square values are not using the value of adjacent diamonds correctly
	for (i = size - 1; i >= 2; i /= 2, range *= pow(2,-factor)) {
		for (j = 0; j < size - 1; j += i) {
			for (k = 0; k < size - 1; k += i) {
				float a = access_2df_array(ret->heightMap, size, j, k);
				float b = access_2df_array(ret->heightMap, size, j, k+i);
				float c = access_2df_array(ret->heightMap, size, j+i, k);
				float d = access_2df_array(ret->heightMap, size, j+i, k+i);

				float e = access_2df_array(ret->heightMap, size, j + i/2, k + i/2) = (a + b + c + d)/4.0f + randomValue(range);

				access_2df_array(ret->heightMap, size, j, k+i/2) = (access_2df_array(ret->heightMap, size, j, k) + access_2df_array(ret->heightMap, size, j, k+i) + e)/3.0f + randomValue(range);
				access_2df_array(ret->heightMap, size, j+i/2, k) = (access_2df_array(ret->heightMap, size, j, k) + access_2df_array(ret->heightMap, size, j+i, k) + e)/3.0f + randomValue(range);
				access_2df_array(ret->heightMap, size, j+i, k+i/2) = (access_2df_array(ret->heightMap, size, j+i, k) + access_2df_array(ret->heightMap, size, j+i, k+i) + e)/3.0f + randomValue(range);
				access_2df_array(ret->heightMap, size, j+i/2, k+i) = (access_2df_array(ret->heightMap, size, j, k+i) + access_2df_array(ret->heightMap, size, j+i, k+i) + e)/3.0f + randomValue(range);
			}
		}
	}

	ret->model = malloc(sizeof(mesh));
		
	mesh_init(ret->model);
	ret->model->indexCount = 6 * (ret->size - 1) * (ret->size - 1);
	ret->model->vertexCount = ret->size * ret->size;

	GLfloat* vertices = malloc(sizeof(float) * ret->model->vertexCount * 8);
	GLuint *indices = malloc(sizeof(GLuint) * ret->model->indexCount);

	for (i = 0, k = 0; i < ret->size; i++)
		for (j = 0; j < ret->size; j++, k+=8) {
			vertices[k] = (-ret->size/2 + i);
			vertices[k+1] = access_2df_array(ret->heightMap, ret->size, i, j);
			vertices[k+2] = (ret->size/2 - j);
			vertices[k+3] = (float)((int)j % 2);
			vertices[k+4] = (float)((int)i % 2);
			vertices[k+5] = 0;
			vertices[k+6] = 1;
			vertices[k+7] = 0;
			// TODO: fix normals
		}

	for (i = 0, k = 0; i < ret->size - 1; i++) {
		for (j = 0; j < ret->size - 1; j++, k+=6) {
			indices[k] = i + ret->size*j;
			indices[k+1] = i + ret->size + 1 + ret->size*j;
			indices[k+2] = i + 1 + ret->size*j;
		}
	}

	for (i = 0, k = 3; i < ret->size - 1; i++) {
		for (j = 0; j < ret->size - 1; j++, k+=6) {
			indices[k] = i + ret->size*j;
			indices[k+1] = ret->size + i + ret->size*j;
			indices[k+2] = i + ret->size + 1 + ret->size*j;
		}
	}

	mesh_loadToVAO(ret->model, vertices, indices);
	mesh_textureFromFile(ret->model, texturePath);

	free(indices);
	free(vertices);

    return ret;
}

void terrain_destroy(terrain *toDestroy) {
	free(toDestroy->model);
	free(toDestroy->heightMap);
	free(toDestroy);
}
